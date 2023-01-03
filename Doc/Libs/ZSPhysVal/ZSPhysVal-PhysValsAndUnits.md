@page _PAGE_Libs_ZSPhysVal_PhysValsAndUnits Physical Values and Units

A physical quantity is a quantitatively determinable property of a physical object.
It is either directly measurable (measuring variable) or can be calculated from other
measuring variables (derived variable).
Physical laws convey the relationship between physical quantities.

The distinguishing feature between similar physical quantities is their value or measured value,
which is given as the product of a numerical value (also called a measure) and a unit of measure.
Independent quantities together with all quantities that can be derived from them form a system of sizes.

# Size Type

The definition of a variable requires the specification of a reproducible measurement specification.
If the same (or an equivalent) measurement specification can be used to define different quantities,
then the quantities are said to be of the same kind. Sizes can only be meaningfully compared, added and
subtracted quantitatively within the same type of size.

For example, the width, height, and length of a cuboid, the diameter of a pipe, the wavelength, and so on
are all length-type quantities because they can all be measured using the same measurement specification.

# Size Value

The distinguishing feature between sizes of the same type of size is their size value. This describes a
specific property of an object quantitatively and thus allows objects with the same property to be compared.

# Numerical Value and Unit

The size value is determined technically via the process of a measurement. Here, the ratio of the size value
to the value of a similar, fixed and well-defined comparison size is determined. The comparative value is called
a unit of measure or unit for short, the quotient of the values of the variable to be quantified and the comparative
variable is called a numerical value or measure. The value of a quantity can then be represented as the product
of a numerical value and a unit.

The size value is determined either by a direct measurement or from a calculation from other measured variables.
The definition of a unit is subject to human arbitrariness. In theory, it is sufficient to define a single unit
for a size type. For historical reasons, however, a large number of different units have often formed for the same
type of size. These differ only by a pure numerical factor.

# Formula and Unit Signs

In mathematical equations, a physical quantity is assigned a character that is called a formula character.
This is basically arbitrary, but there are a number of conventions (e.g. DIN 1304) for designating certain sizes.
A symbol usually consists of just one letter, which can be provided with an index for further differentiation.

There are also standardized characters for units, which are called unit symbols. They usually consist of one or
more Latin letters or more rarely of a special character such as a degree sign.

The size value is always given as a product of the numerical value and the unit.

Since the numerical value depends on the selected unit of measurement, the sole representation of the symbol is
not unambiguous. For this reason, the representation `G/[G]` (e.g. `m/kg`) or `G in [G]` (e.g. `m in kg`) is usual
for labeling tables and coordinate axes.

# Formatting

Formatting is regulated by DIN 1338. Accordingly, the symbol is written in italics, while the unit symbol is written
upright to distinguish it from the symbol. For example, `m` is the symbol for the quantity `mass` and `m` is the unit
symbol for the unit of measurement `meter`.

A space is written between the dimension and the unit sign. An exception to this rule are the degree signs, which are
written without a space directly after the dimension (`an angle of 180°`), provided that no other unit signs follow
(`the outside temperature is 23 °C`).

# Erroneous Sizes

In the case of erroneous values, the numerical value is given with its measurement uncertainty, mostly in the form of
the mean error or sometimes – if known – the maximum error.

Identification is most often done by adding a `±` after the erroneous numerical value, followed by the error value
(where parentheses are required if a unit follows to refer to both values).

    P = (12.34 ± 0.23) mW

The number of uncertain decimal places to be specified for the numerical value depends on the error value.
If it begins with a 1 or 2, two digits are noted, otherwise only one. If necessary, the numerical value has to be rounded.

It is also not unusual to specify the measurement uncertainty as a ratio, e.g. in percent.

# Resolution

If the quantity values ​​are not measured quantities, but setting parameters, they cannot be made with any degree of accuracy.
In this case one speaks more of a setting accuracy or resolution of the size value. If you have set the parameter according
to its resolution, this also happens with a certain accuracy. For setting parameters, both their resolution and their accuracy
would have to be taken into account. However, the resolution of a setting parameter is always "rougher" than the accuracy.
After all, it makes no sense to offer a setting value with a resolution of "0.001 volt" if the value is actually set in the
device with an accuracy of only "0.1 volt". Basically, the situation is the same for both the resolution and the measurement
uncertainty. In the case of setting parameters, the values ​​also have an error – setting uncertainty.

When it comes to the number of decimal places to be specified, however, a distinction must be made between measured and set values.
It does not make sense for setting parameters to display more digits than specified by the resolution of the value.
Furthermore, setting parameters can be changed incrementally in steps by users of a measuring device via an input field.
The increment must be based on the resolution of the setting values. For example, if a parameter can only be set with an accuracy
of 0.25 volts, a setting value of 2.1 volts is not possible. Instead, only values ​​that are an integer divisible by 0.25 are possible
(e.g. 7.0 -> 7.25 -> 15.75 etc.).

In general, setting parameters have a maximum and a minimum value. The increment according to the resolution thus begins at the
minimum possible value. For example, if a voltage can only be set in the range -4 V to +5 V with a resolution of 1.5 V, only the
values -4.0 -2.5 -1.0 0.5 2.0 3.5 and 5.0 are possible.

# Link between physical quantities

## Size Equations

The representation of natural laws and technical relationships in mathematical equations is called quantity equations.
The symbols in a quantity equation have the meaning of physical quantities unless they are intended as symbols for mathematical
functions or operators. Equations of magnitude are valid regardless of the choice of units.

Size equations link different physical sizes and their size values. For the evaluation, the symbol must be replaced by the
product of the numerical value and the unit. The units used are irrelevant. However, the magnitude type must match on both
sides of the equals sign for the equation to make physical sense.

Examples:

    F = ma      Power = Mass * Acceleration
    P = UI      Power = Voltage * Amperage
    R = U/I     Resistance = Voltage / Amperage
    U = RI      Voltage = Resistance * Amperage
    P = I²R     Poser = (Amperage in square) * Resistance
    R = P/I²    Resistance = Power / (Amperage in square)

## Calculation Rules

Not all arithmetic operations that would also be possible with pure numbers make sense for physical quantities.
It has been shown that a small number of calculation rules is sufficient to describe all known natural phenomena.

Addition and subtraction is only possible between quantities of the same type of quantity.

Multiplication and division of different sizes as well as with pure numbers are possible without restrictions.
The product or the quotient is often a new physical quantity. This means that powers with integer exponents are
also allowed. Taking the square root of a quantity is only possible if the quantity can be represented as the
product of two quantities of the same type.

Transcendent functions such as "exp", "log", "sin", "tanh", etc. are only defined for pure numbers and are
therefore only possible for dimensionless quantities. These dimensionless quantities include the logarithmic
ratios such as neper, bel and decibel, as they are used, for example, to indicate volume or in communications
engineering to indicate power.

# Size and Unit Systems

## Size Systems

Every field of knowledge in technology and natural sciences is described by a limited set of physical quantities
that are linked to one another by natural laws. The underlying sizes form a size system.

The quantities of this system are divided into basic quantities and derived quantities. The difference is that the
derived quantities can be represented as power products of the base quantities, while this is not possible with the
base quantities. This classification is largely arbitrary and mostly done for practical reasons. The number of base
sizes determines the degree of the size system. For example, the international sizing system with its seven base
sizes is a seventh degree sizing system

## Systems of Units

A unit is required for each size in order to be able to specify the size value. Therefore, each size system
corresponds to a unit system of the same degree, which is composed analogously of inseparable basic units and
further divisible derived units.

The units derived from the base units are represented by products of powers, but in contrast to size systems,
they may be supplemented by a number factor. The system of units is said to be coherent (connected) if all units
can be formed without this additional factor. In such systems, all quantity equations can be interpreted as
numerical value equations and evaluated accordingly quickly.

The International System of Units (SI) used in almost every country in the world is a coherent system of units
of the seventh degree based on the International System of Sizes. The SI also defines standardized prefixes
for units of measurement. However, the multiples or parts of an SI unit formed in this way are not themselves
part of the actual system of units, as this would contradict coherence. For example, a fictional system of
units that includes the base units centimeter (cm) and second (s) and the derived unit meter per second (m/s)
is not coherent: Because

@f$ 1 \frac m s = {100} \frac {cm} s @f$

one needs a number factor (100) in the formation of this system.

## Special Sizes

**Quotient and Ratio Quantities**

The quotient of two quantities is a new quantity. Such a variable is referred to as a ratio variable if the
output variables are of the same type, otherwise as a quotient variable.

Ratios are fundamentally dimensionless. The name of a ratio usually includes an adjective such as relative
or standardized, or it ends in -number or -value. Examples are the Reynolds number and the CW value.

Only in rare cases do different ratios belong to the same type of quantity. Sometimes, therefore, the unit
symbols are not truncated for better separation when specifying their magnitude value. Ratios are often given
in units of %, ‰ or ppm. Ratio units have a special position if they are the ratio of equal units.

These are always 1 and therefore idempotent, i.e. they can be multiplied by themselves any number of times
without changing their value. Some idempotent ratio units have special names, such as the angle unit radiant (rad).
In coherent systems of units, the ratio units are always 1, i.e. idempotent.

Idempotent ratio units are interesting because you can simply multiply the numerical values ​​here. If one says
that 0.3 part of the earth's surface is landmasses and the continent of Asia represents a 0.3 part of the landmass,
one can conclude that 0.09 of the earth's surface is covered by the continent of Asia because here we use the unit 1,
which is idempotent.

In contrast, percentages are not idempotent. For example, if one says that 30% of the earth's surface is landmasses
and the continent of Asia represents 30% of the landmass, one cannot conclude that 900% of the earth's surface is
covered by the continent of Asia because % is not idempotent, so %2 is not the same as % is.

In many technical fields, the logarithmic ratios are of particular interest. Such quantities are referred to as levels
or measures. If the natural logarithm is used in the formation, this is identified by the auxiliary unit neper (Np),
if it is the decadic logarithm, the auxiliary unit bel (B) or, more often, its tenth, the decibel (dB) is used.

## Logarithmic Units

The *Bel* (B) is an auxiliary unit of measurement, named after Alexander Graham Bell, to indicate gauges and measures.
These logarithmic variables are used, among other things, in acoustics (e.g. sound pressure level, sound reduction index),
high-frequency technology as part of communications technology (e.g. SNR), audio technology and automation technology.
In practice, the use of the tenth part of a bel (decibel, unit symbol dB) is common.

The Bel is used to identify the decade logarithm of the ratio of two similar power or energy quantities P1 and P2:

@f$ L = (\lg \frac {P_2} {P_1})\ B = {10} (\lg \frac {P_2} {P_1})\ {dB} @f$

For L we get the value of a Bel (B) when the power ratio @f$ \frac {P_2} {P_1} = 10 @f$.
The more common decibel (dB) is formed using the unit prefix "deci" (symbol "d"):

@f$ 1\ {dB} = \frac {1} {10}\ B @f$

In linear systems, the power or energy quantities P are proportional to the squares of the effective effective values
of field quantities x (e.g. electrical voltage, sound pressure), this means:

@f$ P \sim \tilde x^2 @f$

If a level or dimension is to be calculated based on field variables, the ratio of the squares of these variables is
obtained and it applies:

@f$ L = {10}(\lg \frac {P_2} {P_1})\ {dB} = {10} (\lg \frac {\tilde x^2_2} {\tilde x^2_1})\ {dB} = {20} (\lg \frac {\tilde x_2} {\tilde x_1})\ {dB} @f$

It should be noted that the argument of the lg function must be a dimensionless quantity, i. H. the quantities
@f$ P_1 @f$ and @f$ P_2 @f$ or @f$ x_1 @f$ and @f$ x_2 @f$ must always have the same unit. An example of a variable
defined in this way is the sound pressure level.

**Conversion to the unit neper**

Decibels and nepers have a fixed linear relationship to each other:

@f$ L = {20} \lg \frac {\tilde x_2} {\tilde x_1}\ {dB} = \ln \frac {\tilde x_2} {\tilde x_1}\ {Np}@f$

By converting to dB and converting the logarithm base, it follows:

@f$ 1\ {dB} = \frac {\ \ln {\frac {\tilde x_2} {\tilde x_1}} {Np}} {\frac {20} {\ln {10}} \ln {\frac {\tilde x_2} {\tilde x_1}}} = \frac {\ln {10}} {20}\ {Np} \approx \frac 1 {8.686}\ {Np} \approx {0.115}\ {Np} @f$

**Decibels and nepers, historical development**

Although not the Bel or Decibel, but the Neper is the coherent auxiliary unit for logarithmic ratios in the
International System of Units SI, the Decibel is predominantly used in practice. On the one hand, there are
historical reasons for this: In the USA, the auxiliary unit of measurement “Mile Standard Cable” (m.s.c.) was
used until 1923 as the unit for the attenuation of a telephone connection. This unit corresponds to the
attenuation of a certain type of cable ("19 gauge") with a length of one English mile and a frequency of 800 Hz
and at the same time the average subjective perception threshold when comparing two volumes.

The latter also applies to the decibel. Therefore, using the decibel resulted in approximately the same numerical
values ​​as using "Mile Standard Cable" (1 m.s.c. = 0.9221 dB).

Another reason for the preferred use of the decibel is that it results in numerical values ​​that are easier to grasp.
For example, doubling the power of about a 3 dB change, tenfold a 10 dB change.

**Use with other units of measure, suffix**

Like any other unit of measure, the bel or decibel can be used in conjunction with other units of measure when
describing a quantity in which a level or measure is multiplied or divided by another quantity. Examples of this
are the attenuation factor of a line in decibels per meter (dB/m) or the related sound power level in decibels
per square meter (dB/m2).

Although it is incorrect under the rules of calculation for quantities to attach suffixes to a unit to convey
information about the nature of the quantity under consideration, such suffixes are common with the decibel.
Because of the clarity and the possible risk of confusion with unit products (e.g. dB•m instead of dBm),
this information must always be linked to the quantity and not to the unit.

The most common examples of dB tags are summarized below:

- @f$ {dBu} @f$
  Voltage level with the reference quantity @f$ \sqrt {{600}\ \Omega * {0.001}\ W} \approx {0.7746}\ V @f$ <br/>
  Suffix at size:           @f$ L_u({re}\ {0.775}\ V) = {...}\ {dB} @f$ <br/>
  Suffix an unit symbol:    @f$ L_u = {...}\ {dB}({0.775}\ V) @f$ <br/>

- @f$ {dBV} @f$
  Voltage level with the reference quantity 1 V<br/>
  Suffix at size:           @f$ L_v({re}\ {1}\ V) = {...}\ {dB} @f$ <br/>
  Suffix an unit symbol:    @f$ L_v = {...}\ {dB}(V) @f$ <br/>

- @f$ {dBm} @f$
  Power level with the reference quantity 1 mW<br/>
  Suffix at size:           @f$ L_p({re}\ {1\ {mW}}) = {...}\ {dB} @f$ <br/>
  Suffix an unit symbol:    @f$ L_p = {...}\ {dB}(1{mW}) @f$ <br/>

- @f$ {dB\mu} @f$
  Level of the electric field strength with the reference value @f$ 1 \frac {1 \mu V} m @f$ <br/>
  Suffix at size:           @f$ L_E({re}\ \frac {1 \mu V} {m}) = {...}\ {dB} @f$ <br/>
  Suffix an unit symbol:    @f$ L_E = {...}\ {dB}(\frac {1 \mu V} {m}) @f$ <br/>

