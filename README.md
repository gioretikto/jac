<h1>JAC 1.6.0 - Scientific command line calculator</h1>

<p>This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.</p>

<p>This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.</p>

<p>You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.</p>

<h2>USAGE</h2>

<p>This is an Infix Notation Calculator for the GNU/Linux Operating System (not a polish one). JAC is the acronym of "Just Another Calculator" and it is particularly suitable for scientifical calculations. Examples of usage of jac are</p>

<pre>cos(3*(pi/2))
1.836685814385754238e-19
cos((3*pi)/2)</pre>

<pre>cos(pi/4*e) = -0.534686930168567074466785728029</pre>

<pre>cos(pi/4*e^2) = 0.887071465339717571814006158931</pre>

<pre>cos(pi/4*e^2)*2 = 1.774142930679435143628012</pre>

<pre>sin pi + 2 + 5 = 7</pre>

<pre>sin (pi/2 + 3 + 5) = -0.1455000338086135254</pre>

<pre>sin (pi/2 + 3 + 5)/2 = -0.07275001690430676268</pre>

<pre>sin(2^(3+2)) = 0.5514266812416905506615581</pre>

<p>Evaluation of the rest energy of the electron</p>

<pre>m_e * c_0^2 = 8.187105776823886314e-14</pre>

<p>You can use scientific notation by using the capital "E", since "e" indicates Euler's number.</p>

<pre>h* 2.5E5 = 1.6565172E-28</pre>

<p>other examples:</p>

<pre>4000*ln2 or 4000ln2 = 2772.5887222397811449071</pre>

<pre>(5^5 -5)/5 + 5 = 629</pre>

<pre>tan(50+1)*3 = 2.709258448126293239</pre>

<p>or</p>

<pre>4+4.5-(34/(8*3-3)) = 6.8809523809523809525</pre>

<pre>100 exp^-(ln2)1000/1590</pre>

<p>You can also pass the expression as argument at the command line</p>

<pre>jac 7*8</pre>

<p>If you pass the expression at the command line, please be mindful that bash treats parenthesis as special charachters therefore escaping is needed:</p>

<pre>jac '4+4.5-(34/(8*3-3))'</pre>

<p>The syntax for other mathematical operators is the following</p>

	- acos(x) Computes the angle (0 to π radians) whose cosine is x

	- asin(x) Computes the angle (—π/2 to π/2 radians) whose sine is x

	- atan(x) Computes the angle (−π/2 to π/2 radians) whose tangent is x

	- cos(x) Computes the cosine of x (x in radians)

	- sin(x) Computes the sine of x (x in radians)

	- tan(x) Computes the tangent of x (x in radians)

	- e^x Computes the exponential function of x (e<sup>x</sup>)
	
	- sinh(x) Computes hyperbolic sine of x
	
	- cosh(x) Computes the hyperbolic cosine of x
	
	- tanhh(x) Computes the hyperbolic tangent of x

	- ln(x) Computes the natural logarithm of x

	- log(x) Computes the base 10 logarithm of x

	- sqrt(x) Computes the square root of x
	
	- n! Computes factorial of n
	
	- cbrt(x) Computes the cube root of x
	
	- bin_dec(x) Conversion of binary x to decimal
	
	- dec_bin(x) Conversion of decimal x to binary
	
	- abs(x) absolute value of x

<p>Remember to use parenthesis with trigonometric functions like sin(pi/2) and not sin pi/2.</p>
	
<h2>Constants</h2>

- pi = 3.14159265358979323846

- c_0 = 2.99792458e8 m/s (exact)	Speed of light in vacuum

- q = 1.602176634e-19 				C elementary charge

- h = 6.62606896e-34    			Planck's constant j⋅s

- m_e = 9.1093837015e-31 			Electron mass kg

- m_p = 1.6726219236951e-27 		Proton mass kg

- e_0 = 8.854187817e-12    			Permittivity of free space (exact)  ^2/N⋅m^2

- n_a = 6.02214179e23				Avogadros's number particles/mol

- k = 1.3806504e-23					Boltzmann's constant j/K

<pre>>>cos(pi)
-1
</pre>

## Get Source and Build

```
git clone https://github.com/gioretikto/jac.git
cd jac
make
sudo make install
```

<h3>Contacts</h3>

<p>All the best,
Giovanni Resta <giovannirestadev@gmail.com></p>
