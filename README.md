<h1>WebDash plugin: Report Repo State</h2>
Reports if the directory contains changes from git's perspective.
<h2>Example (webdash.config.json):</h2>
<pre><code>{
    "commands": [
        {
            "name": "git-status",
            "actions": [
                "git status"
            ]
        },
        {
            "name": "report-repo-state",
            "actions": [
                "report-repo-state git-status"
            ],
            "frequency": "daily",
            "when": "new-day",
            "notify-dashboard": true,
            "wdir": "$.thisDir()"
        }
    ]
}</code></pre>
