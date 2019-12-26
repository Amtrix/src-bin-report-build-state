<h1>WebDash plugin: Report Repo State</h2>
Checks if a command's output contains error information.
<h2>Example:</h2>
<pre><code>{
    "name": "report-repo-state",
    "actions": [
        "git status"
    ],
    "frequency": "daily",
    "when": "new-day",
    "notify-dashboard": true,
    "wdir": "$.thisDir()"
}</code></pre>
