<h1>JAC 1.3 - Just Another Calculator</h1>

<p>This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.</p>

<p>This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.</p>

<p>You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.</p>

<h2>USAGE</h2>

<p>This is an Infix Notation Calculator for the GNU/Linux Operating System (not a polish one). jac is particularly suitable for scientifical calculations. For example the rest energy of the electron is</p>

<pre>m_e * m_0 = 8.187105776823886314e-14</pre>

<p>You can use scientific notation "e"</p>

<pre>h* 2.5e5 =
1.6565172e-28
</pre>

<p>other examples:</p>

<pre>4000*ln2 or 4000ln2 = 2772.5887222397811449071</pre>

<pre>tan(50+1)*3</pre>

<p>or</p>

<pre>4+4.5-(34/(8*3-3)) = 6.8809523809523809525</pre>
<pre>100 exp^-(ln2)1000/1590</pre>

<p>You can also pass the expression as argument at the command line</p>

<pre>jac 7*8</pre>

<p>If you pass the expression at the command line, please be mindful that bash treats parenthesis as special charachters therefore escaping is needed:</p>

<pre>jac 4+4.5-\(34/\(8*3-3\)\)</pre>

<p>The syntax for other mathematical operators is the following</p>

	- acos(x) Returns the angle (0 to π radians) whose cosine is x

	- asin(x) Returns the angle (—π/2 to π/2 radians) whose sine is x

	- atan(x) Returns the angle (−π/2 to π/2 radians) whose tangent is x

	- cos(x) Returns the cosine of x (x in radians)

	- sin(x) Returns the sine of x (x in radians)

	- tan(x) Returns the tangent of x (x in radians)

	- e^x Returns the exponential function of x (e<sup>x</sup>)

	- ln(x) Returns the natural logarithm of x

	- log(x) Returns the base 10 logarithm of x

	- sqrt(x) Returns the square root of x
	
	- n! Returns factorial of n
	
	- cbrt(x) Returns the cube root of x
	
	- bin_dec(x) Conversion of binary x to decimal
	
	- dec_bin(x) Conversion of decimal x to binary
	
	-abs(x) absolute value of x
	
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
```

<h3>Contacts</h3>

<p>All the best,
Giovanni Resta <giovannirestadev@gmail.com></p>
