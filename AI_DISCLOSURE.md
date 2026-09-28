# Generative AI Use Disclosure

## Tool and date

I used ChatGPT (Codex, GPT-6-based) on September 27, 2026. I used it to generate the initial C program and tests, review the result, expand the tests, and draft the documentation. I also received AI help formulating the first two prompts in an earlier conversation before sending them in the code-generation chat. The following three messages are copied exactly from that chat; the backslashes before asterisks in Prompt 1 are part of the message I sent.

## Exact prompts

### Prompt 1

Write a C11 program that reads one line at a time until the user enters exactly `END`. Implement this function exactly:
int extractIPv4(const char\* str, unsigned long\* outAddress, int\* outPort);
Scan the text for maximal consecutive runs containing only ASCII digits, periods, and colons. Validate each entire run; never accept a valid prefix of a malformed run. A valid run has four decimal octets separated by periods, optionally followed immediately by `:port`. Each octet has 1–3 digits, a value from 0 to 255, and no leading zero unless it is `0`. A port has 1–5 digits, a value from 0 to 65535, and the same leading-zero rule. An invalid port invalidates the whole run. Return the first fully valid run. On failure, set the address to 0 and the port to -1.
Compute each number character by character. Do not use string-to-number conversion functions, address-parsing libraries, or regular expressions. Build the 32-bit address value from its four octets. On success, print exactly `Extracted IPv4 address: A.B.C.D (decimal value: N, port: P)` where P is the number or `none`. On failure, print `Invalid input: no valid IPv4 address found`. Prompt with `Enter a string (or 'END' to quit): ` and print `Program terminated.` after END.
Provide `main.c`, a separate `tests.c` with meaningful valid and invalid cases, and commands to compile and run them. Explain your parsing decisions and any assumptions.

### Prompt 2

Critically review the attached C code against the IPv4 extraction rules from our previous prompt. Do not assume that compiling or passing its current tests proves correctness. Trace the code through malformed adjacent punctuation, too many or too few octets, empty fields, leading zeros, numeric limits, invalid ports, and a later valid candidate after an invalid one.
Identify any specific bug with a concrete input, expected result, and actual result. Add independent tests for cases the original tests missed. If you find a bug, provide the smallest justified correction and explain it. If you find none, say so plainly; do not invent a bug. Also check the exact output format and list any remaining ambiguity or limitation. Distinguish what you verified by reasoning from what you actually executed.

### Prompt 3

#### **AI usage and disclosure requirements**

You are required to use a generative AI tool for all or at least part of this assignment's code generation, and to document that usage professionally. The purpose is not to test whether you can write this parser unassisted — it is to test whether you can direct an AI tool effectively, evaluate its output critically, and communicate that process clearly.

#### General disclosure

- Clearly state which GAI tool was used (e.g., ChatGPT, Copilot, Gemini, etc.), and specify the version if known (e.g., GPT-4, Claude 3.5).
- Indicate the date(s) the tool was consulted.

#### Code attribution

- Copy the exact prompt(s) you used to generate the code.
- Document which parts of the code were AI-generated versus student-written.
- Note any modifications you made to the AI-generated output, and why.

#### Verification statement

- Confirm that you understand every line of the submitted code — no blind copying.
- State that the code has been tested and works as intended.
- Acknowledge any known bugs, limitations, or unexpected behavior you did not resolve.

## Code attribution and changes

ChatGPT generated the initial `main.c` and `tests.c` in response to Prompt 1. Prompt 2 asked it to challenge the code with malformed input and boundaries; that review found no error in the IPv4 parser. Prompt 3 supplied the disclosure requirements. In a later follow-up, ChatGPT added 16 cases to `tests.c`, changed `main.c` to print `Program terminated.` only after the user enters `END` rather than after end-of-file, and drafted `README.md` and this disclosure. I supplied the requests and reviewed the result; I did not independently write source code or claim that the AI-generated tests were my own designs.

## Critical review and testing

I understand every line of the submitted code. `is_digit` and `is_run_char` define the characters that can belong to a candidate. `extractIPv4` scans an entire consecutive run before calling `valid_run`; this prevents accepting `192.168.1.1` from inside the malformed run `192.168.1.1..`. `number` accumulates digits manually and rejects leading zeros, excessive length, and values above the field's limit. `valid_run` requires all four octets, validates the entire optional port, and accepts the run only if no characters remain. It then packs the four octets into the 32-bit value. `main` handles the input loop and output format.

I personally ran the program with the five AI-suggested manual inputs below and checked the behavior against the grammar. These cases were supplied by AI and executed by me:

| Input | Behavior checked |
| --- | --- |
| `noise=203.0.113.7:443!` | Extracts `203.0.113.7`, decimal `3405803783`, port `443`; surrounding garbage is skipped. |
| `192.168.1.1..` | Rejects the whole run; a valid prefix is insufficient. |
| `1.2.3.4:65536 / 172.16.0.1` | Rejects the first run because the port exceeds `65535`, then extracts `172.16.0.1`, decimal `2886729729`, with no port. |
| `8.8.8.8:00053` | Rejects the entire run because the port has a leading zero. |
| `10.0.0.1:80:` | Rejects the entire run because of the extra colon. |

The repository's `tests.c` contains 31 assertions, including zero and maximum values, malformed punctuation, leading zeros, invalid ports, and a later valid candidate. ChatGPT compiled the submitted files as C11 with `-Wall -Wextra -Wpedantic -Werror` and ran that suite; all cases passed. It also checked the assignment's sample run and the exact success, failure, and `END` messages. My own run and these AI-assisted checks worked as intended for the specified inputs. I did not accept the code solely because it compiled: I examined how candidate boundaries and port validation prevent partial matches.

## Known limitations

If one line contains multiple valid candidates, the function returns the first. The C-string interface stops at an embedded NUL byte, so later bytes would not be scanned. I know of no remaining bug for the assignment's ordinary text input.
