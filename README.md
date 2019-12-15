<h1>WebDash plugin: Report Build State</h2>
Checks if a command's output contains error information.
<h2>Example:</h2>
<pre><code>{
      "name": "report-build-state",
      "actions": [
          "report-build-state clean",
          "report-build-state setup-project",
          "report-build-state build-server",
          "report-build-state build-client"
      ],
      "frequency": "daily",
      "wdir": "$.thisDir()"
  }</code></pre>
