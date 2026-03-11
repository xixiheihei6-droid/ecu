---
layout: null
---

<style>
  * { box-sizing: border-box; }
  body {
    margin: 0;
    background: #fff;
    color: #000;
    font: 16px/1.5 -apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif;
  }
  .page {
    max-width: 960px;
    margin: 0 auto;
    padding: 24px 16px 40px;
  }
  a { color: #000; }
  img, video {
    max-width: 100%;
    height: auto;
  }
</style>

<div class="page">
  {% capture readme %}{% include_relative README.md %}{% endcapture %}
  {{ readme | markdownify }}
</div>
