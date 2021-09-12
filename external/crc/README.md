# libcrcle

CRC checksum library supporting 32-bit variants used
in Bullfrog and Terminal Reality assets.

based on source code from:

- [macutils](http://github.com/dgilman/macutils)
- [rhash](http://github.com/rhash/RHash)

# Status

CRC32                   : CRC32       - 0xedb88320 - 0xffffffff
CRC32C                  : CRC32C      - 0x82f63b78 - 0xffffffff
CRC-CCITT32             : CRC-CCITT32 - 0x04c11db7 - 0xffffffff
ZIP     		: ZIP         - 0xedb88320 - 0xffffffff

# Build

```c
cmake .
make install
```

# Usage
crcchk [OPTION]... [FILE]...
Output CRC checksum file of listed files in option defined style.

      --crc32           use CRC32 algorithm
      --crc32c          use CRC32C algorithm
      --ccitt32         use CCITT32 algorithm
      --zip             use ZIP style of algorithm
  -t, --table      	print CRC table.
      --poly=POLYGON	use POLYGON to generate table.
      --start=START	use START as starting value for CRC.
      --simple      	print message digests, using simple format (default).
      --sfv         	print message digests, using SFV format.
      --bsd         	print message digests, using BSD-like format.


