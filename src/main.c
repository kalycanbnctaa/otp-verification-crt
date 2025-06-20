#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <locale.h>

#define MAX 10
#define LARGE_PRIME 10000019
#define MAX_ATTEMPTS 3
#define LOCK_TIME 5

// Rate limiting structure
typedef struct {
    int failed_attempts;
    time_t last_attempt;
} RateLimiter;

// Initialize rate limiter
void init_rate_limiter(RateLimiter *rl) {
    rl->failed_attempts = 0;
    rl->last_attempt = 0;
}

// Check and enforce rate limiting
int check_rate_limit(RateLimiter *rl) {
    time_t current_time = time(NULL);
    
    // If too many failures and not enough time has passed
    if (rl->failed_attempts >= MAX_ATTEMPTS && 
        (current_time - rl->last_attempt) < LOCK_TIME) {
        printf("Too many attempts. Please wait %d seconds.\n", 
               LOCK_TIME - (int)(current_time - rl->last_attempt));
        return 1; // Limited
    }
    
    // Reset if lock time has passed
    if ((current_time - rl->last_attempt) >= LOCK_TIME) {
        rl->failed_attempts = 0;
    }
    
    return 0; // Not limited
}

uint32_t secure_hash(int T, int ID) {
    uint64_t h = ((uint64_t)T ^ 0xDEADBEEF) * ((uint64_t)ID | 0xCAFEBABE);

    h ^= h >> 23;
    h *= 0x2127599BF4325C37ULL;
    h ^= h >> 47;
    
    return (uint32_t)(h % LARGE_PRIME);
}

// Extended Euclidean Algorithm
int extendedEuclid(int a, int b, int *x, int *y) {
    if (b == 0) {
        *x = 1; *y = 0;
        return a;
    }
    int x1, y1;
    int gcd = extendedEuclid(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return gcd;
}

// Modular inverse
int modInverse(int a, int m) {
    int x, y;
    int g = extendedEuclid(a, m, &x, &y);
    return (g == 1) ? (x % m + m) % m : -1;
}

// Chinese Remainder Theorem
long long CRT(int a[], int m[], int n) {
    long long M = 1;
    for (int i = 0; i < n; i++) {
        if (m[i] <= 1) {
            printf("Modulus must be > 1. Aborting.\n");
            return -1;
        }
        M *= m[i];
    }

    if (M <= LARGE_PRIME) {
        printf("Error: Keyspace (M) must exceed LARGE_PRIME. Aborting.\n");
        return -1;
    }

    long long result = 0;
    for (int i = 0; i < n; i++) {
        long long Mi = M / m[i];
        long long yi = modInverse(Mi, m[i]);
        if (yi == -1) {
            printf("Moduli are not pairwise coprime. Aborting.\n");
            return -1;
        }
        result += (long long)a[i] * Mi * yi;
    }
    return result % M;
}

void save_rate_limiter(RateLimiter *rl) {
    FILE *f = fopen("ratelimit.log", "w");
    if (f) {
        fprintf(f, "%d %ld\n", rl->failed_attempts, rl->last_attempt);
        fclose(f);
    }
}

void load_rate_limiter(RateLimiter *rl) {
    FILE *f = fopen("ratelimit.log", "r");
    if (f) {
        fscanf(f, "%d %ld", &rl->failed_attempts, &rl->last_attempt);
        fclose(f);
    }
}

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    int n;
    int a[MAX], m[MAX];
    int T, ID;
    RateLimiter rl = {0, 0}; 
    load_rate_limiter(&rl); 

    printf("=== OTP Generation and Verification ===\n");

    // Check rate limit before proceeding
    if (check_rate_limit(&rl)) {
        return 1;
    }

    printf("Enter current time (e.g., timestamp): ");
    scanf("%d", &T);

    printf("Enter user ID: ");
    scanf("%d", &ID);

    printf("Enter number of moduli (n): ");
    scanf("%d", &n);

    printf("Enter %d pairwise coprime moduli:\n", n);
    for (int i = 0; i < n; i++) {
        printf("Modulus m[%d]: ", i + 1);
        scanf("%d", &m[i]);
    }

    // Generate expected value using secure hash
    uint32_t expected = secure_hash(T, ID);

    // Generate residues
    for (int i = 0; i < n; i++) {
        a[i] = (expected % m[i] + m[i]) % m[i];
    }

    printf("\nResidue values (OTP):\n");
    for (int i = 0; i < n; i++) {
        printf("x ≡ %d mod %d\n", a[i], m[i]);
    }

    // Reconstruct x using CRT
    long long x = CRT(a, m, n);

    if (x == -1) {
        return 1;
    }

    printf("\nReconstructed x: %lld\n", x);
    printf("Expected x (from secure hash): %u\n", expected);

    long long user_input;
    int verified = 0;
while (rl.failed_attempts < MAX_ATTEMPTS && !verified) {
    printf("\nEnter OTP to verify: ");
    scanf("%lld", &user_input);

    rl.last_attempt = time(NULL);

    if (user_input == expected) {
        printf("OTP is VALID\n");
        rl.failed_attempts = 0;
        verified = 1;
    } else {
        rl.failed_attempts++;
        int attempts_left = MAX_ATTEMPTS - rl.failed_attempts;
        printf("OTP is INVALID\n");
        if (attempts_left > 0) {
            printf("Attempts left: %d\n", attempts_left);
        } else {
            printf("Too many failed attempts. Try again in %d seconds.\n", LOCK_TIME);
        }
    }

    save_rate_limiter(&rl);
}


if (!verified && rl.failed_attempts >= MAX_ATTEMPTS) {
    printf("Locking for %d seconds...\n", LOCK_TIME);
    sleep(LOCK_TIME);
}

    return 0;
}
