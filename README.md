### Virtualized Input Detection

Figured I could share this because finding code on the MS documentation is close to non-existant.

## What does it do?

The copy-pasted code from multiple sources does the following:

- Spawn a window the size of your default screen width/height
- Print on the console window whether or not the input was virtualized

# Notes

Virtualized input could only be detected if the input was performed inside of the launched window.
This is running on user-level, the detector itself could be at risk of result manipulation.