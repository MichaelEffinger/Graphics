# Namespaces, Classes oh my!

## Namespaces
## `namespace ES`
This is the main namespace for everything within the purview of this project. The std of this dog and pony show.
**All subsequent namespaces listed are implied to have ES:: tacked on.**
### -`math`
Utility math header which contains a lot of good stuff. 
- ### `math::angle_literals`
A cheeky little namespace which adds _deg and _rad as literals for ease of use.
For example, `auto A = 90_deg * 2` would lend you an angle object of 180°.
### -`Secret`
This is the detail, impl, priv, or what-have-you of ES. Anything inside here you are ill-advised to call. Abandon all hope, ye who enter here.


---
## Classes
*All classes listed live in the ES namespace unless specified otherwise*
### `VectorN`
The most elaborate class... to say the least...
### `angle`
A class which represents an angle in either degrees or radians, strongly typed, but with plenty of implicit conversions!  
`angle<in_radians> x = angle<in_degrees>(45) - angle<in_radians>(std::numbers:pi_v<float>)` lends x to be -2.35619 radians, easy!
