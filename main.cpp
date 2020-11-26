/**
 * # RECSOLVER
 * solves second degree recurrences with constant coefficients
 * This program solves the recurrence relations in the form
 * 	a(n) = r a(n−1) + s a(n−2) + t
 * where r, s,t and the first two terms a(0) , a(1) of the sequence are supplied by the user as integer constants.
 * We assume that s != 0.
 *
 * 
 * author: Jayanth PSY
 *      19CS10068,
 *      Computer Science and Engineering Department, Indian Institute of Technology Kharagpur
 *
 * social:
 * https://github.com/j-tesla/
 */

#include <iostream>
#include <algorithm>
#include <stdexcept>

/** \brief searches for an integer n between start and end, both inclusive such that n * n = m
 *
 * @param m integer of which sqrt is to be found
 * @param start pass 0 by default
 * @param end pass m by default
 * @return sqrt(m) if m is perfect square else -1
 */
int sqrt(int m, int start, int end) {
    if (start > end)
        return -1;
    if (start == end) {
        if (start * start == m)
            return start;
        else
            return -1;
    } else {
        int mid = (start + end) / 2;
        int mid_sq = mid * mid;
        if (mid_sq == m)
            return mid;
        else if (mid_sq > m)
            return sqrt(m, start, mid - 1);
        else
            return sqrt(m, mid + 1, end);
    }
}

/** \brief Datatype to handle roots if quadratic equations with integral coefficients without float approximations
 *
 *  implemented operations doesn't handle exhaustive cases,
 *  but are intended to handle solving second order recurrence relation
 */
class splnum {
public:
    splnum() = default;

    /** \brief (a + b * root(c) ) / d
     *
     * Can assume rational / irrational / complex values
     * @param a
     * @param b
     * @param c under root part
     * @param d denominator
     */
    splnum(int a, int b, int c, int d = 1) : c(c) {
        if (d == 0) {
            throw std::runtime_error("Math error: Division by zero is undefined\n");
        } else if (d < 0) {
            d = -d;
            a = -a;
            b = -b;
        }
        int gcd_ = std::__gcd(std::__gcd(a, b), d);
        this->a = a / gcd_;
        this->b = b / gcd_;
        this->d = d / gcd_;

        checkC();
    }

    /** \brief a special case of splnum in the form of a (integer type)
     *
     * supports implicit conversion from integers
     * @param a integer
     */
    splnum(int a) {
        this->a = a;
        this->b = this->c = 0;
        this->d = 1;
    }

    /** \brief  a special case of splnum in the form of a / d (rational number)
     *
     * @param a numerator
     * @param d denominator
     */
    splnum(int a, int d) {
        if (d == 0) {
            throw std::runtime_error("Math error: Division by zero is undefined\n");
        } else if (d < 0) {
            d = -d;
            a = -a;
            b = -b;
        }
        int gcd_ = std::__gcd(a, d);
        a /= gcd_;
        d /= gcd_;
        if (d < 0) {
            a = -a;
            d = -d;
        }
        this->a = a;
        this->b = this->c = 0;
        this->d = d;
    }

    /** \brief reciprocal of the number
     *
     * @return
     */
    splnum inverse() const {
        return splnum(d * a, -d * b, c, a * a - b * b * c);
    }

    splnum operator-() const {
        return splnum(-a, -b, c, d);
    }

    splnum operator+(const splnum &other) const {
        if (c == 0 or b == 0) {
            return splnum(a * other.d + other.a * d, d * other.b, other.c, d * other.d);
        } else if (other.c == 0 or other.b == 0) {
            return splnum(other.a * d + a * other.d, other.d * b, c, other.d * d);
        } else if (c == other.c) {
            return splnum(a * other.d + other.a * d, b * other.d + other.b * d, c, d * other.d);
        } else {
            throw std::runtime_error("Error: Operating different kinds of splnums is not supported");
        }
    }

    splnum operator-(const splnum &other) const {
        return this->operator+(-other);
    }

    splnum operator*(const splnum &other) const {
        if (c == 0 or b == 0) {
            return splnum(a * other.a, a * other.b, other.c, d * other.d);
        } else if (other.c == 0 or other.b == 0) {
            return splnum(other.a * a, other.a * b, c, other.d * d);
        } else if (c == other.c) {
            return splnum(a * other.a + b * other.b * c, a * other.b + other.a * b, c, d * other.d);
        } else {
            throw std::runtime_error("Error: Operating different kinds of splnums is not supported");
        }
    }

    splnum operator/(const splnum &other) const {
        return this->operator*(other.inverse());
    }

    bool operator==(const splnum &other) const {
        return (c == other.c) and (a * other.d == d * other.a) and (b * other.d == d * other.b);
    }

    bool operator!=(const splnum &other) const {
        return not this->operator==(other);
    }

    splnum &operator=(const splnum &other) = default;

    friend std::ostream &operator<<(std::ostream &out, const splnum &x);

private:
    int a, b, c, d;

    /// \brief checks whether c is a perfect square to bring it out from under the square-root
    void checkC() {
        if (c != 0) {
            int sqrt_c = sqrt(c, 0, 1);
            if (sqrt_c != -1) {
                c = 0;
                a = a + b * sqrt_c;
                b = 0;
                int gcd_ = std::__gcd(a, d);
                a /= gcd_;
                d /= gcd_;
            }
        }
    }

};

std::ostream &operator<<(std::ostream &out, const splnum &x) {

    if (x.a != 0) {
        if (x.b != 0 and x.c != 0)
            out << "(";
        out << x.a;
        if (x.b > 0 and x.c != 0) {
            out << " + ";
            if (x.b != 1)
                out << x.b << " ";
        } else if (x.b < 0 and x.c != 0) {
            out << " - ";
            if (x.b != -1)
                out << -x.b << " ";
        }
    } else if (x.b != 0 and x.c != 0) {
        if (x.b == -1)
            out << "-";
        else if (x.b == 1);
        else {
            out << x.b << " ";
        }
    }
    if (x.c != 0 and x.b != 0)
        out << "sqrt(" << x.c << ")";

    if (x.a != 0 and x.b != 0 and x.c != 0)
        out << ")";
    if (x.d != 1)
        out << " / " << x.d;
    return out;
}

/** \brief solves the equation x^2 - rx - s = 0
 *
 * @param r
 * @param s
 * @param roots passed by reference for the solution
 */
void findroots(int r, int s, splnum roots[2]) {
    int delta = r * r + 4 * s;
    int sqrt_delta = sqrt(delta, 0, delta);
    if (sqrt_delta == -1) {
        roots[0] = splnum(r, 1, delta, 2);
        roots[1] = splnum(r, -1, delta, 2);
    } else {
        roots[0] = splnum(r + sqrt_delta, 0, 0, 2);
        roots[1] = splnum(r - sqrt_delta, 0, 0, 2);
    }
}

/** \brief solves recurrence relation in the form a(n) = r a(n-1) + s a(n-2)
 *
 * prints the characteristic equation, its roots and the Homogeneous solution of the recurrence
 * @param r
 * @param s
 * @param a0 base case: a(0)
 * @param a1 base case: a(1)
 */
void solvehomogeneous(int r, int s, int a0, int a1) {
    std::cout << "\n+++ Solving the homogeneous recurrence\n";
    splnum roots[2];
    std::cout << "    Characteristic equation: ";
    std::cout << "x^2 + (" << -r << ")x + (" << -s << ") = 0\n";
    findroots(r, s, roots);
    std::cout << "    Root 1 = " << roots[0] << "\n";
    std::cout << "    Root 2 = " << roots[1] << "\n";

    std::cout << "    Homogeneous solution :\n    ";
    if (roots[0] != roots[1]) {                         // roots are distinct
        splnum u = (roots[1] * a0 - a1) / (roots[1] - roots[0]);
        splnum v = (roots[0] * a0 - a1) / (roots[0] - roots[1]);

        std::cout << "[" << u << "] [" << roots[0] << "]^n + [" << v << "] [" << roots[1] << "]^n\n";
    } else {
        splnum u = a0;
        splnum v = splnum(a1) / roots[0] - u;
        std::cout << "[(" << v << ")n + (" << u << ")] [" << roots[0] << "]^n\n";
    }
}

/** \brief solves recurrence relation in the form a(n) = r a(n-1) + s a(n-2) + t
 *
 * prints the characteristic equation, its roots and the Particular abd Homogeneous parts of the solution of the recurrence
 * @param r
 * @param s
 * @param t
 * @param a0
 * @param a1
 */
void solvenonhomogeneous(int r, int s, int t, int a0, int a1) {
    std::cout << "\n+++ Solving the nonhomogeneous recurrence";
    splnum roots[2];
    std::cout << "    Characteristic equation: ";
    std::cout << "x^2 + (" << -r << ")x + (" << -s << ") = 0\n";
    std::cout << "    Characteristic equation: ";
    std::cout << "x^2 + (" << -r << ")x + (" << -s << ") = 0\n";
    findroots(r, s, roots);
    std::cout << "    Root 1 = " << roots[0] << "\n";
    std::cout << "    Root 2 = " << roots[1] << "\n";

    std::cout << "    Particular solution : ";
    splnum a0_ = a0, a1_ = a1;

    if (roots[0] != 1 and roots[1] != 1) {
        splnum constant = splnum(t, 1 - r - s);
        std::cout << constant;
        a0_ = splnum(a0) - constant;
        a1_ = a1_ - constant;
    } else if (roots[0] == 1 and roots[1] == 1) {
        splnum n_coeff = splnum(-t, r + 4 * s);
        std::cout << "[" << n_coeff << "] n^2";
        a1_ = a1_ - n_coeff;
    } else {
        splnum n2_coeff = splnum(t, r + 2 * s);
        std::cout << "[" << n2_coeff << "] n";
        a1_ = a1_ - n2_coeff;
    }

    std::cout << "\n    Homogeneous solution :\n    ";
    if (roots[0] != roots[1]) {                         // roots are distinct
        splnum u = (roots[1] * a0_ - a1_) / (roots[1] - roots[0]);
        splnum v = (roots[0] * a0_ - a1_) / (roots[0] - roots[1]);

        std::cout << "[" << u << "] [" << roots[0] << "]^n + [" << v << "] [" << roots[1] << "]^n\n";
    } else {
        splnum u = a0_;
        splnum v = splnum(a1_) / roots[0] - u;
        std::cout << "[(" << v << ")n + (" << u << ")] [" << roots[0] << "]^n\n";
    }
}


int main() {

    // scanning input:
    int r, s, t, a0, a1;
    std::cout << "r = ";
    std::cin >> r;
    std::cout << "s = ";
    std::cin >> s;
    std::cout << "t = ";
    std::cin >> t;
    std::cout << "a0 = ";
    std::cin >> a0;
    std::cout << "a1 = ";
    std::cin >> a1;

    // solving the recurrence
    solvehomogeneous(r, s, a0, a1);
    solvenonhomogeneous(r, s, t, a0, a1);
    return 0;
}

