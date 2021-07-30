# hioctane
Hi-Octane port in C

new map list format using simple checksum 
with additional comment line as name

create maps.crc:

```c
cd data && rhash -C -simple maps/*
```

add map name as a comment beginning with:

```c
; <name>
```

after the line following the checksum and file
