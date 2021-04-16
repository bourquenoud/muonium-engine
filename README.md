# Muonium Engine
Muonium is a highly customisable 3D/2D rendering engine for microcontrollers 
incorporating multiple kind of optimisations.

# 3D rendering
## Configurable to work in floating or fixed point
It is possible to configure Muonium to work exclusively with fixed point arithmetic.
The difference is not noticeable, and computation is significantly faster on MCU
without FPU.

#### Fixed point render
![Fixed point render of a teapot](https://raw.githubusercontent.com/elzaidir/muonium-engine/main/image/muonium-teapot-fixed.png)

#### Floating point render
![Floating point render of a teapot](https://raw.githubusercontent.com/elzaidir/muonium-engine/main/image/muonium-teapot-fixed.png)

However, computation with fixed point is bit slower than floating point if the system
has a FPU, about 10%.

# 2D rendering
In progess...

# About
## Origin
A while ago I started experimenting with 3D and graphics in general on PIC32, and
 wanted to make a game console with 3D capabilities. My console (somewhat) works,
  and I wanted to make a more structured code and open it for everyone to use 

## Why this name ?
*Muonium is an exotic atom made up of an antimuon(μ) and an electron(e) [...]
 muonium(μe)* (Wikipedia)

Micro (graphic) Engine -> μe -> Muonium. Yup. Thank you for coming to my TED 
talk.

# Disclaimer
The software is provided "as is", without warranty of any kind, express or implied,
including but not limited to the warranties of merchantability, fitness for a
particular purpose and noninfringement. in no event shall the authors be liable
for any claim, damages or other liability, whether in an action of contract, tort 
or otherwise, arising from, out of or in connection with the software or the use
or other dealings in the software.
