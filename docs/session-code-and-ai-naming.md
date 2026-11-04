# Session code folders and AI filenames

By default each IDE startup creates a persistent directory under the platform
Documents folder: `RedPandaCPP/Sessions/<local timestamp>_<unique id>`. If Documents
is unavailable, the home directory is used. Directories are never automatically
deleted. Restored and explicitly opened files retain their paths; project files
keep their normal project workflow.

## New file saving settings

Open **Tools > Options > Editor > New File Saving**. The settings are saved in the
IDE configuration and apply to standalone files created after clicking Apply or
OK. No restart is needed. Existing editors retain their paths and first-save
behavior.

| Setting | Choices |
| --- | --- |
| Save folder | Any absolute path; an empty field uses the default Documents path |
| Subfolder rule | A separate folder for each launch (default), one folder per local calendar day, or the selected folder directly |
| Filename rule | Numbered names such as `untitled1.cpp` (default), or local timestamp names such as `code_2026-11-04_12-30-00-123_1.cpp` |
| First Save | Save directly using the default name (default), or open Save As in the selected folder |

The settings page previews an example path. The source type determines the
extension (C, C++, assembly or text). Daily folders are selected when each file
is created, so new files created after midnight use the next day's folder.

Direct first saves reserve a free name exclusively before writing. Occupied
names receive a numeric suffix. Later saves update the same file. Save As remains
available for a custom location. If a destination folder cannot be created, the
IDE reports it and falls back to Save As. Cancelling Save As keeps the editor
contents and does not create a file. The AI panel's explicit Save with this name
button always saves to an unused name next to the current source file.

## AI filename panel

Open **View > AI filename** and choose a provider:

| Provider | Default full chat completions URL | Suggested models |
| --- | --- | --- |
| OpenAI | `https://api.openai.com/v1/chat/completions` | `gpt-4o-mini`, `gpt-4.1-mini`, `gpt-4.1` |
| DeepSeek | `https://api.deepseek.com/chat/completions` | `deepseek-chat`, `deepseek-reasoner` |
| Custom (OpenAI compatible) | Enter your provider's full HTTPS URL | Enter your provider's model ID |

The model selector is editable; presets are examples, not a list of supported
models guaranteed by the provider. DeepSeek's `/v1/chat/completions` URL also
works. APIs must accept OpenAI Chat Completions `messages` and return
`choices[0].message.content`. Native Responses, Anthropic or Gemini protocols
are not supported by this client.

Enter the selected provider's API key. Switching providers clears the key and
remembers the endpoint and model for each provider during the session. The active
provider's non-secret configuration is saved when a valid generation request is
sent. Older manually configured endpoints and models are preserved. The key
remains in memory for this run only. The API service may charge for requests.
Provider fields are locked while a request is running.

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
