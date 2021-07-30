# hioctane
Hi-Octane port in C

addition of maps.crc

create maps.crc:

```c
cd data && rhash -C -simple maps/*
```

add map name as a comment beginning with:

```c
; <name>
```

after the line following the checksum and file
