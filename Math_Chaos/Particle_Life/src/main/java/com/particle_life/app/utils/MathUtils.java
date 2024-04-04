package com.particle_life.app.utils;

/**
 * A utility class for mathematical operations.
 */
public final class MathUtils {

    /**
     * Constrains an integer value within a specified range.
     *
     * @param value the value to constrain
     * @param min the minimum value
     * @param max the maximum value
     * @return the constrained value
     */
    public static int constrain(int value, int min, int max) {
        if (value < min) {
            return min;
        } else if (value > max) {
            return max;
        } else {
            return value;
        }
    }

    /**
     * Constrains a double value within a specified range.
     *
     * @param value the value to constrain
     * @param min the minimum value
     * @param max the maximum value
     * @return the constrained value
     */
    public static double constrain(double value, double min, double max) {
        if (value < min) {
            return min;
        } else if (value > max) {
            return max;
        } else {
            return value;
        }
    }

    /**
     * Calculates the modulo of two integers.
     *
     * @param a the dividend
     * @param b the divisor
     * @return the modulo result
     */
    public static int modulo(int a, int b) {
        if (a < 0) {
            do {
                a += b;
            } while (a < 0);
            return a;
        } else if (a >= b) {
            do {
                a -= b;
            } while (a >= b);
            return a;
        }
        return a;
    }

    /**
     * Performs linear interpolation between two double values.
     *
     * @param a the start value
     * @param b the end value
     * @param f the interpolation factor
     * @return the interpolated value
     */
    public static double lerp(double a, double b, double f) {
        return a + (b - a) * f;
    }

    /**
     * Returns <code>Math.round(value)</code> instead of <code>Math.floor(value)</code>
     * if <code>value</code> is closer to the next integer than <code>threshold</code>.
     *
     * @param value the value to adjust
     * @param threshold some positive value like 0.001
     * @return the adjusted value
     */
    public static double tolerantFloor(double value, double threshold) {
        double x = Math.round(value);
        if (Math.abs(x - value) < threshold) {
            return x;
        }
        return Math.floor(value);
    }

    /**
     * Returns <code>Math.round(value)</code> instead of <code>Math.ceil(value)</code>
     * if <code>value</code> is closer to the previous integer than <code>threshold</code>.
     *
     * @param value the value to adjust
     * @param threshold a positive value like 0.001
     * @return the adjusted value
     * @param threshold
     * @return
     */
    public static double tolerantCeil(double value, double threshold) {
        double x = Math.round(value);
        if (Math.abs(x - value) < threshold) return x;
        return Math.ceil(value);
    }
}

