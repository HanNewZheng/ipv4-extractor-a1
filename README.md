# A1: Extracting IPv4 Addresses from Noisy Text

This C11 program reads lines until `END`, finds the first complete valid IPv4 token in each line, and prints its dotted form, decimal value, and optional port.

## Build and run

```sh
cc -std=c11 -Wall -Wextra -Wpedantic main.c -o ipv4
./ipv4
```

## Run the tests

```sh
cc -std=c11 -Wall -Wextra -Wpedantic -DIPV4_NO_MAIN main.c tests.c -o tests
./tests
```

## How it works

`extractIPv4` skips characters outside `[0-9.:]`. Each consecutive run of those characters is checked as a whole. `number` reads digits manually and rejects leading zeros, too many digits, and values above the allowed limit. `valid_run` requires four octets and, if present, a complete valid port. It packs the octets into a 32-bit value by shifting eight bits per octet. A malformed run is rejected in full; scanning then continues with the next run.

On extraction failure, the function sets the address to `0` and the port to `-1`. A successful address without a port also uses `-1` for the port. See `AI_DISCLOSURE.md` for the prompts, attribution, verification, and limitations.
