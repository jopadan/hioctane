# hioctane
Hi-Octane port in C

# Dependencies
- libzip http://github.com/nih-at/libzip/
- libcrc http://github.com/jopadan/libcrc/

# Building

```c
cmake .
make
make install
```

# Status

new map list format using simple checksum 
with additional comment line as name

create maps.cfg:

```c
cd data && rhash -C -simple maps/*
```

add map name as a comment beginning with:

```c
; <name>
```

after the line following the checksum and file


required header information to create entities.cfg:

```c
<hexadecimal type size> <hexadecimal subtype size>
; types count / subtypes count
<hexadecimal type> <hexadecimal subtype> <decimal enum int id>
; Unknown neutral element
```

for every entity

```c
<hexadecimal type> <hexadecimal subtype> <decimal enum int id>
; <entity name string>
```

must be added past the header of entities.cfg

