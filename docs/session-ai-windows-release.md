# Windows x64 session AI build

This is a custom prerelease, not an official upstream Red Panda C++ release.
It ships the optional Session folders and AI filenames plugin in
`fileplugins/sessionai.dll`.

## Run

Extract the entire ZIP and start `RedPanda-CPP/RedPandaIDE.exe`. Keep all DLLs,
plugins, templates and helper programs together. Windows 10 version 1809 or
later (64-bit) is required. Qt and MSVC runtime libraries are included.

The archive does not include GCC/MinGW, GDB or AStyle. Configure an installed
C/C++ toolchain in the IDE settings to compile and debug programs. Configure
AStyle separately if code formatting is needed.

Enable **Tools > Plugins > Session folders and AI filenames**. Use the two tabs
in the plugin panel to configure new-file saving and request a filename from
OpenAI, DeepSeek or a custom compatible service. Turn the same switch off to
restore the original new-file and save workflows immediately. Only clicking
Generate sends source code. API keys are kept in memory while the plugin is
enabled. See `session-code-and-ai-naming.md` for details.

## Interface language

Use Tools > Options > Environment > Appearance > Language to choose the IDE
language. Save the settings and restart the IDE for translations to take effect.
The interface font and font size can be selected on the same page.

## Build and release

The `Release Windows session AI build` GitHub Actions workflow runs for tags
matching `session-ai-v*`. It can also be run manually with an existing tag.
It builds with MSVC and Qt 6.8.3, runs editor, provider selection and filename tests, checks required
package files, computes SHA-256 checksums and publishes a GitHub prerelease only
when the build and tests succeed. The release job has repository contents write
permission; the build job has read permission. No personal API token is needed.

The workflow file must exist in the source selected by the tag. Fork owners may
need to enable Actions in their repository before the first run. For manual runs,
the workflow must also be present on the repository's default branch.
