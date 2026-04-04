import sys
import re

with open('c:/Users/HP/Desktop/FIA/umbrella-project.org-1/fiaphy/index.html', 'r', encoding='utf-8') as f:
    text = f.read()

# Find start
matches_start = list(re.finditer(r'<section [^>]*>\s*<h2 class=\"fiaphy-section-title\">Platform Availability</h2>', text))
if not matches_start:
    print('COULD NOT FIND START STR')
    sys.exit(1)

start_idx = matches_start[0].start()

# Find end
matches_end = list(re.finditer(r'<section [^>]*>\s*<h2 class=\"fiaphy-section-title\">Frequently Asked Questions</h2>', text))
if not matches_end:
    print('COULD NOT FIND END STR')
    sys.exit(1)

end_idx = matches_end[0].start()

if end_idx <= start_idx:
    print('END IS BEFORE START')
    sys.exit(1)

new_content = '''<section class=\"fiaphy-section\" style=\"background: var(--bg-secondary);\">
        <h2 class=\"fiaphy-section-title\">Installing Platforms</h2>
        
        <div style=\"max-width: 900px; margin: 0 auto; color: var(--text-secondary); line-height: 1.8;\">
            <h3 style=\"color: var(--text-primary); margin-top: 2rem;\">Arduino IDE Library Manager</h3>
            <p>Open your Arduino IDE, then navigate to sketch, click on include library, and then click on manage libraries. Search for \"FiaPhy\" in the search box and then click the install button.</p>

            <h3 style=\"color: var(--text-primary); margin-top: 2rem;\">Manual ZIP File Installation</h3>
            <p>You can download the latest FiaPhy ZIP release from GitHub. Open your Arduino IDE, go to sketch, click include library, and choose add .ZIP library. Select the downloaded ZIP file to install it.</p>
            <p><a href=\"https://github.com/fiaos-org/fiaphy/releases\" target=\"_blank\">Download from GitHub Releases</a></p>

            <h3 style=\"color: var(--text-primary); margin-top: 2rem;\">VS Code Extension</h3>
            <p>Open Visual Studio Code, go to extensions, search for \"FiaPhy Language Support\", and click the install button. You can also get it from the <a href=\"https://marketplace.visualstudio.com/items?itemName=FiaOS.fiaphy-language-support\" target=\"_blank\">VS Code Marketplace</a>.</p>

            <h3 style=\"color: var(--text-primary); margin-top: 2rem;\">PlatformIO</h3>
            <p>If you are using PlatformIO, add the repository directly to your platformio.ini configuration file under lib_deps like this:</p>
            <pre><code>[env:yourboard]
platform = atmelavr
board = uno
lib_deps = fiaos-org/FiaPhy@^1.0.1</code></pre>
            <p>Alternatively, you can install it via the command line by executing: <code>pio lib install fiaos-org/FiaPhy@^1.0.1</code></p>
        </div>
    </section>

    '''

with open('c:/Users/HP/Desktop/FIA/umbrella-project.org-1/fiaphy/index.html', 'w', encoding='utf-8') as f:
    f.write(text[:start_idx] + new_content + text[end_idx:])

print('Successfully updated fiaphy index.html')
