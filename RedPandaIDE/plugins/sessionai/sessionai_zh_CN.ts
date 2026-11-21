<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
    <context>
        <name>AiFilenameWidget</name>
        <message>
            <source>Generate sends the current file's code to your API. Review the name, then save. Existing files are kept as copies.</source>
            <translation>点击生成会将当前文件的代码发送至你设置的 API。检查文件名后再保存，已有文件将保留副本。</translation>
        </message>
        <message>
            <source>Kept only for this session</source>
            <translation>仅在本次运行中保留</translation>
        </message>
        <message>
            <source>Filename without extension</source>
            <translation>文件名（不含扩展名）</translation>
        </message>
        <message>
            <source>Generate filename</source>
            <translation>生成文件名</translation>
        </message>
        <message>
            <source>Save with this name</source>
            <translation>按此名称保存</translation>
        </message>
        <message>
            <source>API URL</source>
            <translation>API 地址</translation>
        </message>
        <message>
            <source>Model</source>
            <translation>模型</translation>
        </message>
        <message>
            <source>API key</source>
            <translation>API 密钥</translation>
        </message>
        <message>
            <source>Name</source>
            <translation>文件名</translation>
        </message>
        <message>
            <source>Select a standalone source file first.</source>
            <translation>请先选择独立的代码文件（非项目文件）。</translation>
        </message>
        <message>
            <source>Use a nonempty file smaller than 128 KiB.</source>
            <translation>请选择非空且不超过 128 KiB 的文件。</translation>
        </message>
        <message>
            <source>Enter an HTTPS chat completions URL, model and API key.</source>
            <translation>请输入 HTTPS 聊天补全 API 地址、模型和密钥。</translation>
        </message>
        <message>
            <source>Generating…</source>
            <translation>正在生成……</translation>
        </message>
        <message>
            <source>Request timed out. Try again.</source>
            <translation>请求超时，请重试。</translation>
        </message>
        <message>
            <source>API request failed (HTTP %1). Check the URL, model, key and network.</source>
            <translation>API 请求失败（HTTP %1）。请检查地址、模型、密钥和网络。</translation>
        </message>
        <message>
            <source>The API did not return a filename. Try again.</source>
            <translation>API 未返回文件名，请重试。</translation>
        </message>
        <message>
            <source>The API returned an invalid filename.</source>
            <translation>API 返回的文件名无效。</translation>
        </message>
        <message>
            <source>Review the name before saving. The source extension is preserved.</source>
            <translation>请检查文件名后保存，原文件扩展名将保留。</translation>
        </message>
        <message>
            <source>The source changed or closed. Generate a new name first.</source>
            <translation>代码已变更、切换或关闭，请重新生成文件名。</translation>
        </message>
        <message>
            <source>Enter a valid filename.</source>
            <translation>请输入有效的文件名。</translation>
        </message>
        <message>
            <source>Cannot create file: %1</source>
            <translation>无法创建文件：%1</translation>
        </message>
        <message>
            <source>Could not save the file.</source>
            <translation>无法保存文件。</translation>
        </message>
        <message>
            <source>Saved: %1</source>
            <translation>已保存：%1</translation>
        </message>
        <message>
            <source>Provider</source>
            <translation>服务商</translation>
        </message>
        <message>
            <source>Custom (OpenAI compatible)</source>
            <translation>自定义（OpenAI 兼容）</translation>
        </message>
        <message>
            <source>Full HTTPS chat completions URL.</source>
            <translation>完整的 HTTPS 聊天补全接口地址。</translation>
        </message>
        <message>
            <source>Choose a model or enter the model ID supported by your provider.</source>
            <translation>可选择常用模型，或输入服务商支持的模型 ID。</translation>
        </message>
        <message>
            <source>Enter the API key for the selected provider.</source>
            <translation>请填写当前服务商的 API 密钥。</translation>
        </message>
    </context>
    <context>
        <name>EditorNewFilesWidget</name>
        <message>
            <source>Browse...</source>
            <translation>浏览……</translation>
        </message>
        <message>
            <source>A separate folder for each launch</source>
            <translation>每次启动创建独立目录</translation>
        </message>
        <message>
            <source>One folder per day (yyyy-MM-dd)</source>
            <translation>按日期分组（yyyy-MM-dd）</translation>
        </message>
        <message>
            <source>Use the selected folder directly</source>
            <translation>直接使用所选目录</translation>
        </message>
        <message>
            <source>Numbered names (untitled1.cpp)</source>
            <translation>按编号命名（untitled1.cpp）</translation>
        </message>
        <message>
            <source>Timestamp names (code_date_time.cpp)</source>
            <translation>按时间命名（code_日期_时间.cpp）</translation>
        </message>
        <message>
            <source>Save new files directly using the default name on first Save</source>
            <translation>首次保存新文件时直接使用默认文件名</translation>
        </message>
        <message>
            <source>Save folder:</source>
            <translation>保存根目录：</translation>
        </message>
        <message>
            <source>Subfolder rule:</source>
            <translation>子目录规则：</translation>
        </message>
        <message>
            <source>Filename rule:</source>
            <translation>文件名规则：</translation>
        </message>
        <message>
            <source>When unchecked, the first Save opens Save As in the chosen folder. Occupied names receive a numeric suffix when saving directly. Settings apply to standalone files created afterwards; existing files keep their paths.</source>
            <translation>取消勾选后，首次保存将在所选目录打开“另存为”。直接保存遇到重名时会自动加编号。这些设置用于之后新建的独立文件（非项目文件），已有文件路径保持原样。</translation>
        </message>
        <message>
            <source>Select the code save folder</source>
            <translation>选择代码保存目录</translation>
        </message>
        <message>
            <source>Please choose an absolute save folder path.</source>
            <translation>请选择绝对路径作为保存目录。</translation>
        </message>
        <message>
            <source>Example path: %1</source>
            <translation>示例路径：%1</translation>
        </message>
        <message>
            <source>Apply</source>
            <translation>应用</translation>
        </message>
        <message>
            <source>Save folder</source>
            <translation>保存目录</translation>
        </message>
    </context>
    <context>
        <name>SessionAiPlugin</name>
        <message>
            <source>Session folders and AI filenames</source>
            <translation>会话目录与 AI 文件命名</translation>
        </message>
        <message>
            <source>AI filename</source>
            <translation>AI 文件命名</translation>
        </message>
        <message>
            <source>New File Saving</source>
            <translation>新文件保存</translation>
        </message>
        <message>
            <source>Code folder</source>
            <translation>代码目录</translation>
        </message>
        <message>
            <source>Save Error</source>
            <translation>保存失败</translation>
        </message>
        <message>
            <source>Cannot create the code folder in %1. New files will use Save As.</source>
            <translation>无法在 %1 创建代码目录。新文件将使用“另存为”。</translation>
        </message>
    </context>
</TS>
