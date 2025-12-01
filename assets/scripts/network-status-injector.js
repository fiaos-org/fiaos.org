// Mobile Network Status Injector
// Injects a small mobile-only network status ribbon into any page
(function () {
    document.addEventListener('DOMContentLoaded', function () {
        if (document.querySelector('.mobile-network-status')) return;

        const announcement = document.getElementById('announcementRibbon');

        const wrapper = document.createElement('div');
        wrapper.className = 'mobile-network-status';
        wrapper.innerHTML = `
            <div class="network-status-inner">
                <span class="network-status-label"><i class="bi bi-broadcast"></i> Network:</span>
                <div class="network-status-display">
                    <div class="network-status-item active" data-index="0">
                        <span class="status-dot status-declined" aria-hidden="true"></span>
                        <span class="location">New York, USA</span>
                        <span class="status-text">• Testing</span>
                    </div>
                    <div class="network-status-item" data-index="1">
                        <span class="status-dot status-declined" aria-hidden="true"></span>
                        <span class="location">Sydney, AU</span>
                        <span class="status-text">• Testing</span>
                    </div>
                    <div class="network-status-item" data-index="2">
                        <span class="status-dot status-declined" aria-hidden="true"></span>
                        <span class="location">Colombo, LK</span>
                        <span class="status-text">• Testing</span>
                    </div>
                    <div class="network-status-item" data-index="3">
                        <span class="status-dot status-active" aria-hidden="true"></span>
                        <span class="location">api.fiaos.org</span>
                        <span class="status-text">• LIVE</span>
                    </div>
                    <div class="network-status-item" data-index="4">
                        <span class="status-dot status-active" aria-hidden="true"></span>
                        <span class="location">cloud.fiaos.org</span>
                        <span class="status-text">• LIVE</span>
                    </div>
                </div>
            </div>
        `;

        if (announcement && announcement.parentNode) {
            announcement.parentNode.insertBefore(wrapper, announcement.nextSibling);
        } else {
            document.body.insertBefore(wrapper, document.body.firstChild);
        }

        // Cycling logic (single-item morphing)
        const display = wrapper.querySelector('.network-status-display');
        if (!display) return;
        const items = Array.from(display.querySelectorAll('.network-status-item'));
        let current = 0;

        // Ensure only the first is active initially
        items.forEach((it, i) => {
            it.classList.toggle('active', i === 0);
        });

        function cycle() {
            items[current].classList.remove('active');
            items[current].classList.add('exit');
            current = (current + 1) % items.length;
            setTimeout(() => {
                items.forEach(it => it.classList.remove('exit'));
                items[current].classList.add('active');
            }, 150);
        }

        // Start cycling immediately (works on all screen sizes)
        setInterval(cycle, 3000);

        // Pause cycling while user hovers/touches the ribbon
        let paused = false;
        let pausedInterval = null;
        
        wrapper.addEventListener('pointerenter', () => {
            paused = true;
        });
        
        wrapper.addEventListener('pointerleave', () => {
            paused = false;
        });
    });
})();
