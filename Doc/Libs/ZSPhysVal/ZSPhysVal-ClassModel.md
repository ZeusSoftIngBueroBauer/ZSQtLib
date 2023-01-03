@page _PAGE_Libs_ZSPhysVal_ClassModel Class Model

The ZSPhysVal subsystem of the ZSQtLib is a C++ class library that is intended to make it easier to
deal with physical values and takes into account the theoretical basics described in the introduction.

# Organization of the Units in a hierarchical structured Index Tree

The data model of physical units should reflect the subdivision of the units into the SI base quantities,
into knowledge areas, unit systems, the associated SI base units and the units derived from them.

The SI base quantities and base units are:

Base Size            | Formula Symbol | Symbol for Dimension | Base Units     | Unit Sysbol
-------------------- | -------------- | -------------------- | -------------- | ---------------
Length               | l, s, x, r, .. | L                    | Meter          | m
Mass                 | m              | M                    | Kilogram       | kg
Time                 | t              | T                    | Second         | s
Electric Current     | I              | I                    | Ampere         | A
Absolute Temperature | T              | θ                    | Kelvin         | K
Amount of Substance  | n              | N                    | Mol            | mol
Luminous Intensity   | IV             | J                    | Candela        | cd

Physical units can be divided into eight knowledge areas, for which the following table gives an (incomplete) overview:

Area of Knowledge         | Size Type           | Physical Size                  | Formula Symbol | Dimension     | SI-Base Unit
------------------------- | ------------------- | ------------------------------ | -------------- | ------------- | ------------
Geometry                  | Angle               | Level Angle,                   | α, β, γ, ...   | L             | Radiant (rad)
^                         | ^                   | Rotation Angle                 | φ, θ, ..       | ^             | ^
^                         | Length              | Length                         | l              | L             | Meter (m)
^                         | Length              | Diameter                       | d, D           | L             | Meter (m)
^                         | ...                 | ...                            | ...            | ...           | ...
Kinematics                | Time                | Time, Period of Time, Duration | t              | T             | Sekunde (s)
^                         | Time                | Period Duration                | T, τ           | T             | Sekunde (s)
^                         | Speed               | Speed                          | v, u, w, c     | L/T           | m/s
^                         | Acceleration        | Acceleration                   | a              | L/T2          | m/s2
^                         | Frequenzy           | Frequenzy                      | f, ν           | L/T           | Hertz (Hz)
^                         | ...                 | ...                            | ...            | ...           | ...
Mechanics                 | Mass                | Mass                           | m              | M             | Kilogramm (kg)
^                         | Power               | Power                          | F              | ML/T2         | Newton (N)
^                         | Power               | Friction                       | FR             | ML/T2         | Newton (N)
^                         | Power               | Power                          | P              | ML2/T3        | Watt (W)
^                         | ...                 | ...                            | ...            | ...           | ...
Thermodynamics            | Temperature         | Absolute Temperature           | T              | θ             | Kelvin (K)
^                         | Energy              | Heat, Amount of Heat           | Q              | ML/T2         | Joule (J)
^                         | Energy              | Thermal Energy                 | Eth            | ML/T2         | Joule (J)
^                         | Power               | Heat Flow                      | Фth, Ф, Q      | ML2/T3        | Watt (W)
^                         | ...                 | ...                            | ...            | ...           | ...
Electricity and Magnetism | El. Current         | El. Current                    | I              | I             | Ampere (A)
^                         | El. Voltage         | El. Voltage                    | U              | ML2/T3I1      | Volt (V)
^                         | El. Resistance      | Ohmic Resistance               | R              | ML2/T3I2      | Ohm (Ω)
^                         | Power               | Active Power                   | P              | ML2/T3        | Watt (W)
^                         | ...                 | ...                            | ...            | ...           | ...
Atomic and Molecular      | Amount of Substance | Amount of Substance            | n              | N             | Mol (mol)
^                         | Relative Mass       | Relative Atomic Mass           | Ar             | 1             | Eins
^                         | ...                 | ...                            | ...            | ...           | ...
Nuclear Physics           | Activity            | Activity                       | A              | 1/T           | Becquerel (Bq)
^                         | Time                | Half-Life-TIme                 | T½             | T             | Sekunde (s)
^                         | ...                 | ...                            | ...            | ...           | ...
Photometry and Optics     | Luminous Intensity  | Luminous Intensity             | lV             | J             | Candela (cd)
^                         | Length              | Focal Length                   | f              | L             | Meter (m)
^                         | ...                 | ...                            | ...            | ...           | ...

Furthermore, the quotients and ratios such as percentages must be taken into account.

