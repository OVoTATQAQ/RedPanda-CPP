# Session code folders and AI filenames

Each IDE startup creates a persistent directory under the platform Documents
folder: `RedPandaCPP/Sessions/<local timestamp>_<unique id>`. If Documents is
unavailable, the home directory is used. Directories are never automatically
deleted. Restored and explicitly opened files retain their existing paths;
project files keep their normal project workflow.

New standalone C, C++, assembly and text files use this session directory.
Their first normal Save (Ctrl+S) writes the default name without a filename
dialog. An occupied name gets a numeric suffix, reserved exclusively before
writing. Later saves update that same file. Save As remains available for a
custom location. If the session folder cannot be created, the IDE reports it
and falls back to the original Save As workflow.

## AI filename panel

Open **View > AI filename**. Enter the full HTTPS URL of an OpenAI-compatible
chat completions endpoint (for example `https://api.openai.com/v1/chat/completions`),
a supported model name and your API key. The URL and model are stored in the IDE
settings; the key remains in memory for this run only. The API service may charge
for requests.

Select a standalone file, then click **Generate filename**. Only this action sends
the current editor text to the selected API. Files must be nonempty and at most
128 KiB. Requests time out after 30 seconds. Redirects are not followed.

Review or edit the suggested stem and click **Save with this name**. The source
extension and directory are preserved. Names are restricted to ASCII letters,
digits, underscores and hyphens, with Windows reserved device names protected.
Occupied destinations receive a numeric suffix and are never overwritten by the
panel. For previously saved files this performs Save As: the original remains
on disk, and the editor switches to the newly named copy. Project files are not
supported by this panel. A changed, switched or closed editor requires generating
a fresh suggestion before saving.

The built-in endpoint and model are editable examples; any provider must support
the `messages` request and `choices[0].message.content` response format. API errors
leave the editor contents intact. No API key is bundled with the application.
