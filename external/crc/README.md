# crc

CRC checksum single header library supporting 32-bit variants used
in Bullfrog and Terminal Reality assets.

# Status

Bullfrog Hi-Octane      : CRC32       - edb88320 - ffffffff
                        : CRC32C      - 
Terminal Reality Engines: CRC-CCITT32 - 04c11db7 - ffffffff
			: ZIP         - edb88320 - ffffffff
# Build

```c
cmake .
make install
```

# Usage
crc [OPTION]... [FILE]...
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


