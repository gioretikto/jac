<h1>JAC 0.7 - Just Another Calculator</h1>

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

<p>This is an Infix Notation Calculator for the GNU/Linux Operating System (not a polish one) able to perform calculation such as:</p>

<pre>tan(50+1)*3</pre>

<p>or</p>

<pre>4+4.5-(34/(8*3-3)) = 6.88095238095238137532</pre>

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

	- exp(x) Returns the exponential function of x (e<sup>x</sup>)

	- ln(x) Returns the natural logarithm of x

	- log(x) Returns the base 10 logarithm of x

	- sqrt(x) Returns the square root of x
	
	- n! Returns factorial of n
	
	- bin_dec(x) Conversion of binary x to decimal
	
	- dec_bin(x) Conversion of decimal x to binary
	
<h2>Constants</h2>

- pi = 3.14159265358979323846

<pre>>>cos(pi)
-1
</pre>

<h3>Contacts</h3>

<p>You can reach me on the freenode IRC channel #cansi or giovannirestadev@gmail.com</p>
