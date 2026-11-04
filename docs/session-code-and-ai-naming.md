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
