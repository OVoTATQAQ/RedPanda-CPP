# Windows x64 session AI build

This is a custom prerelease, not an official upstream Red Panda C++ release.
It includes session code folders, OpenAI/DeepSeek/custom API presets and the AI
filename panel. The interface uses point-sized fonts and explicit locale-aware
CJK fallback families to keep Chinese text consistent across controls.

## Run

Extract the entire ZIP and start `RedPanda-CPP/RedPandaIDE.exe`. Keep all DLLs,
plugins, templates and helper programs together. Windows 10 version 1809 or
later (64-bit) is required. Qt and MSVC runtime libraries are included.

The archive does not include GCC/MinGW, GDB or AStyle. Configure an installed
C/C++ toolchain in the IDE settings to compile and debug programs. Configure
AStyle separately if code formatting is needed.

New standalone code files save into a separate Documents/RedPandaCPP/Sessions
folder for each IDE startup. Open View > AI filename to configure your own
OpenAI-compatible HTTPS chat completions API, model and key. Only clicking
Generate sends source code. API keys are kept in memory for the current run.
See `session-code-and-ai-naming.md` for details.

## Switching an English Windows installation to Chinese

Change the IDE language through Tools > Options > Environment > Appearance,
or close the IDE before editing the portable configuration file. For existing
builds, the relevant settings are:

```ini
[Environment]
language=zh_CN
interface_font=Microsoft YaHei UI
interface_font_size=11
```

Use `Microsoft YaHei` if `Microsoft YaHei UI` is not installed. New builds keep
an explicitly selected font and add installed CJK fallbacks for the chosen IDE
language, independently of the Windows system language. Interface font sizes use
points so that Qt handles per-screen scaling.

## Build and release

The `Release Windows session AI build` GitHub Actions workflow runs for tags
matching `session-ai-v*`. It can also be run manually with an existing tag.
It builds with MSVC and Qt 6.8.3, runs editor, provider selection, filename and interface font tests (including
150% scaling), checks required
package files, computes SHA-256 checksums and publishes a GitHub prerelease only
when the build and tests succeed. The release job has repository contents write
permission; the build job has read permission. No personal API token is needed.

The workflow file must exist in the source selected by the tag. Fork owners may
need to enable Actions in their repository before the first run. For manual runs,
the workflow must also be present on the repository's default branch.
