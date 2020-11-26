#include <iostream>
#include <numeric>
#include <stdexcept>

int sqrt(int m, int start, int end) {
}


class splnum {
public:
    splnum() = default;

    splnum(int a, int b, int c, int d = 1) : c(c) {
    }

   
    splnum(int a) {
    }

    splnum(int a, int d) {
    }

    splnum inverse() const {
    }

    splnum operator-() const {
    }

    splnum operator+(const splnum &other) const {
    }

    splnum operator-(const splnum &other) const {
        return this->operator+(-other);
    }

    splnum operator*(const splnum &other) const {
    }

    splnum operator/(const splnum &other) const {
        return this->operator*(other.inverse());
    }

    bool operator==(const splnum &other) const {
    }

    bool operator!=(const splnum &other) const {
        return not this->operator==(other);
    }

    splnum &operator=(const splnum &other) = default;

    friend std::ostream &operator<<(std::ostream &out, const splnum &x);

private:
    int a, b, c, d;
};

std::ostream &operator<<(std::ostream &out, const splnum &x) {
    return out;
}

void findroots(int r, int s, splnum roots[2]) {
}


void solvehomogeneous(int r, int s, int a0, int a1) {
}

void solvenonhomogeneous(int r, int s, int t, int a0, int a1) {
}


int main() {
    return 0;
}

