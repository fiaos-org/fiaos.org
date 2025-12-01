// ============================================
// FIAOS ETSCI+ - MAIN JAVASCRIPT
// Professional Research Institution Website
// ============================================

document.addEventListener('DOMContentLoaded', function() {
    
    // ============================================
    // Cookie Consent Management
    // ============================================
    const cookieConsent = document.getElementById('cookieConsent');
    const acceptCookies = document.getElementById('acceptCookies');
    
    function checkCookieConsent() {
        const hasConsent = localStorage.getItem('fiaos-cookie-consent');
        if (hasConsent === 'accepted') {
            cookieConsent.classList.add('hidden');
        }
    }
    
    if (acceptCookies) {
        acceptCookies.addEventListener('click', function() {
            localStorage.setItem('fiaos-cookie-consent', 'accepted');
            localStorage.setItem('fiaos-consent-date', new Date().toISOString());
            cookieConsent.classList.add('hidden');
        });
    }
    
    checkCookieConsent();
    
    // ============================================
    // Announcement Ribbon Management
    // ============================================
    const announcementRibbon = document.getElementById('announcementRibbon');
    const dismissAnnouncement = document.getElementById('dismissAnnouncement');
    
    function checkAnnouncementDismissed() {
        const dismissed = localStorage.getItem('fiaos-announcement-dismissed');
        const dismissedVersion = localStorage.getItem('fiaos-announcement-version');
        const currentVersion = 'dev-release-v0.1';
        
        // Show again if it's a new announcement version
        if (dismissed === 'true' && dismissedVersion === currentVersion) {
            announcementRibbon.classList.add('hidden');
        }
    }
    
    if (dismissAnnouncement) {
        dismissAnnouncement.addEventListener('click', function() {
            localStorage.setItem('fiaos-announcement-dismissed', 'true');
            localStorage.setItem('fiaos-announcement-version', 'dev-release-v0.1');
            announcementRibbon.classList.add('hidden');
        });
    }
    
    checkAnnouncementDismissed();
    
    // ============================================
    // Theme Toggle
    // ============================================
    const themeToggle = document.getElementById('themeToggle');
    const html = document.documentElement;
    
    // Load saved theme
    const savedTheme = localStorage.getItem('theme') || 'light';
    html.setAttribute('data-theme', savedTheme);
    updateThemeIcon(savedTheme);
    
    themeToggle.addEventListener('click', function() {
        const currentTheme = html.getAttribute('data-theme');
        const newTheme = currentTheme === 'light' ? 'dark' : 'light';
        html.setAttribute('data-theme', newTheme);
        localStorage.setItem('theme', newTheme);
        updateThemeIcon(newTheme);
    });
    
    function updateThemeIcon(theme) {
        const icon = themeToggle.querySelector('i');
        if (theme === 'dark') {
            icon.className = 'bi bi-moon-fill';
        } else {
            icon.className = 'bi bi-sun-fill';
        }
    }
    
    // ============================================
    // Mobile Menu
    // ============================================
    const mobileMenuBtn = document.getElementById('mobileMenuBtn');
    const mobileMenuOverlay = document.getElementById('mobileMenuOverlay');
    const mobileMenuClose = document.getElementById('mobileMenuClose');
    
    if (mobileMenuBtn) {
        mobileMenuBtn.addEventListener('click', function() {
            mobileMenuOverlay.classList.add('active');
            document.body.style.overflow = 'hidden';
        });
    }
    
    if (mobileMenuClose) {
        mobileMenuClose.addEventListener('click', function() {
            mobileMenuOverlay.classList.remove('active');
            document.body.style.overflow = '';
        });
    }
    
    // Close on link click
    const mobileNavLinks = document.querySelectorAll('.mobile-nav-link');
    mobileNavLinks.forEach(link => {
        link.addEventListener('click', function() {
            mobileMenuOverlay.classList.remove('active');
            document.body.style.overflow = '';
        });
    });
    
    // Close on overlay click
    mobileMenuOverlay.addEventListener('click', function(e) {
        if (e.target === mobileMenuOverlay) {
            mobileMenuOverlay.classList.remove('active');
            document.body.style.overflow = '';
        }
    });
    
    // ============================================
    // Full-Screen Search Overlay
    // ============================================
    const searchToggle = document.getElementById('searchToggle');
    const searchOverlay = document.getElementById('searchOverlay');
    const searchClose = document.getElementById('searchClose');
    const searchInput = document.getElementById('searchInput');
    const searchResults = document.getElementById('searchOverlayResults');
    
    if (searchToggle) {
        searchToggle.addEventListener('click', function() {
            searchOverlay.classList.add('active');
            document.body.style.overflow = 'hidden';
            setTimeout(() => {
                searchInput.focus();
            }, 100);
        });
    }
    
    if (searchClose) {
        searchClose.addEventListener('click', function() {
            searchOverlay.classList.remove('active');
            document.body.style.overflow = '';
            searchInput.value = '';
            searchResults.innerHTML = '';
        });
    }
    
    // Close on overlay click
    searchOverlay.addEventListener('click', function(e) {
        if (e.target === searchOverlay) {
            searchOverlay.classList.remove('active');
            document.body.style.overflow = '';
            searchInput.value = '';
            searchResults.innerHTML = '';
        }
    });
    
    // Close on Escape key
    document.addEventListener('keydown', function(e) {
        if (e.key === 'Escape') {
            if (searchOverlay.classList.contains('active')) {
                searchOverlay.classList.remove('active');
                document.body.style.overflow = '';
                searchInput.value = '';
                searchResults.innerHTML = '';
            }
            if (mobileMenuOverlay.classList.contains('active')) {
                mobileMenuOverlay.classList.remove('active');
                document.body.style.overflow = '';
            }
        }
    });
    
    // ============================================
    // Search Functionality
    // ============================================
    let searchTimeout;
    if (searchInput) {
        searchInput.addEventListener('input', function() {
            clearTimeout(searchTimeout);
            const query = this.value.trim();
            
            if (query.length < 2) {
                searchResults.innerHTML = '';
                return;
            }
            
            searchTimeout = setTimeout(() => {
                performSearch(query);
            }, 300);
        });
    }
    
    function performSearch(query) {
        // Placeholder search functionality
        // In a real implementation, this would search through documentation
        searchResults.innerHTML = `
            <div style="padding: 2rem; color: #fff; text-align: center;">
                <p style="margin-bottom: 1rem;">Searching for: <strong>${query}</strong></p>
                <p style="color: rgba(255,255,255,0.6);">Search functionality will be implemented here</p>
            </div>
        `;
    }
    
    // ============================================
    // Smooth Scrolling for Anchor Links
    // ============================================
    document.querySelectorAll('a[href^="#"]').forEach(anchor => {
        anchor.addEventListener('click', function (e) {
            const href = this.getAttribute('href');
            if (href === '#') return;
            
            e.preventDefault();
            const target = document.querySelector(href);
            if (target) {
                const offset = 120; // Account for fixed header
                const targetPosition = target.getBoundingClientRect().top + window.pageYOffset - offset;
                window.scrollTo({
                    top: targetPosition,
                    behavior: 'smooth'
                });
            }
        });
    });
    
    // ============================================
    // Active Sidebar Link on Scroll
    // ============================================
    const sidebarLinks = document.querySelectorAll('.sidebar-link[href^="#"]');
    const sections = document.querySelectorAll('.content-section[id]');
    
    function updateActiveSection() {
        const scrollPosition = window.scrollY + 150;
        
        sections.forEach(section => {
            const sectionTop = section.offsetTop;
            const sectionHeight = section.offsetHeight;
            const sectionId = section.getAttribute('id');
            
            if (scrollPosition >= sectionTop && scrollPosition < sectionTop + sectionHeight) {
                sidebarLinks.forEach(link => {
                    link.classList.remove('active');
                    if (link.getAttribute('href') === `#${sectionId}`) {
                        link.classList.add('active');
                    }
                });
            }
        });
    }
    
    window.addEventListener('scroll', updateActiveSection);
    updateActiveSection();
    
    // ============================================
    // Dynamic Publication Loading (Placeholder)
    // ============================================
    function loadPublications() {
        // This would dynamically load publications from a JSON file or API
        // Placeholder for now
        console.log('Publications loaded');
    }
    
    loadPublications();
    
    // ============================================
    // Console Info
    // ============================================
    console.log('%cFiaOS ETSci+', 'font-size: 18px; font-weight: bold; color: #0a66c2;');
    console.log('%cEnvironmental Intelligence Research', 'font-size: 12px; color: #666;');
    console.log('%cA Meta-affiliated research initiative', 'font-size: 10px; color: #999;');
    
    
});
