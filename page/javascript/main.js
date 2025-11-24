// Smooth scroll behavior for navigation links
document.querySelectorAll('a[href^="#"]').forEach(anchor => {
    anchor.addEventListener('click', function (e) {
        e.preventDefault();
        const target = document.querySelector(this.getAttribute('href'));
        if (target) {
            target.scrollIntoView({
                behavior: 'smooth',
                block: 'start'
            });
        }
    });
});

// Navbar scroll effect
let lastScroll = 0;
const header = document.querySelector('.header');

window.addEventListener('scroll', () => {
    const currentScroll = window.pageYOffset;
    
    if (currentScroll <= 0) {
        header.classList.remove('scroll-up');
        return;
    }
    
    if (currentScroll > lastScroll && !header.classList.contains('scroll-down')) {
        // Scroll Down
        header.classList.remove('scroll-up');
        header.classList.add('scroll-down');
    } else if (currentScroll < lastScroll && header.classList.contains('scroll-down')) {
        // Scroll Up
        header.classList.remove('scroll-down');
        header.classList.add('scroll-up');
    }
    lastScroll = currentScroll;
});

// Intersection Observer for animation on scroll
const observerOptions = {
    threshold: 0.1,
    rootMargin: '0px 0px -100px 0px'
};

const observer = new IntersectionObserver((entries) => {
    entries.forEach(entry => {
        if (entry.isIntersecting) {
            entry.target.classList.add('animate-in');
        }
    });
}, observerOptions);

// Observe all sections for animation
document.querySelectorAll('.innovation-item, .feature-card, .spec-category').forEach(el => {
    observer.observe(el);
});

// Video autoplay handling
document.addEventListener('DOMContentLoaded', () => {
    const video = document.querySelector('.hero-video video');
    if (video) {
        // Ensure video is muted and autoplays
        video.muted = true;
        video.play().catch(error => {
            console.log('Autoplay was prevented:', error);
        });
    }
});

// Dropdown menu functionality (if needed in future)
document.querySelectorAll('.nav-item.dropdown').forEach(item => {
    item.addEventListener('click', function(e) {
        // Toggle dropdown menu
        this.classList.toggle('active');
    });
});

// Form handling (for future contact forms)
function handleFormSubmit(event) {
    event.preventDefault();
    // Add form submission logic here
    console.log('Form submitted');
}

// Add fade-in animation class
const style = document.createElement('style');
style.textContent = `
    .animate-in {
        animation: fadeInUp 0.6s ease-out forwards;
    }
    
    @keyframes fadeInUp {
        from {
            opacity: 0;
            transform: translateY(30px);
        }
        to {
            opacity: 1;
            transform: translateY(0);
        }
    }
    
    .scroll-down {
        transform: translateY(-100%);
        transition: transform 0.3s ease-in-out;
    }
    
    .scroll-up {
        transform: translateY(0);
        transition: transform 0.3s ease-in-out;
    }
`;
document.head.appendChild(style);

// Lazy loading for images
if ('loading' in HTMLImageElement.prototype) {
    const images = document.querySelectorAll('img[loading="lazy"]');
    images.forEach(img => {
        img.src = img.dataset.src;
    });
} else {
    // Fallback for browsers that don't support lazy loading
    const script = document.createElement('script');
    script.src = 'https://cdnjs.cloudflare.com/ajax/libs/lazysizes/5.3.2/lazysizes.min.js';
    document.body.appendChild(script);
}

// Performance optimization: Debounce scroll events
function debounce(func, wait = 10, immediate = true) {
    let timeout;
    return function() {
        const context = this, args = arguments;
        const later = function() {
            timeout = null;
            if (!immediate) func.apply(context, args);
        };
        const callNow = immediate && !timeout;
        clearTimeout(timeout);
        timeout = setTimeout(later, wait);
        if (callNow) func.apply(context, args);
    };
}

// Analytics tracking (placeholder for future implementation)
function trackEvent(category, action, label) {
    // Add analytics tracking code here
    console.log(`Event tracked: ${category} - ${action} - ${label}`);
}

// Track button clicks
document.querySelectorAll('.btn').forEach(button => {
    button.addEventListener('click', function(e) {
        const buttonText = this.textContent.trim();
        trackEvent('Button', 'Click', buttonText);
    });
});

// Mobile menu toggle (for future mobile navigation)
function toggleMobileMenu() {
    const nav = document.querySelector('.nav-menu');
    nav.classList.toggle('mobile-active');
}

// Accessibility: Focus management
document.addEventListener('keydown', (e) => {
    if (e.key === 'Tab') {
        document.body.classList.add('keyboard-navigation');
    }
});

document.addEventListener('mousedown', () => {
    document.body.classList.remove('keyboard-navigation');
});

// Console welcome message
console.log('%c🌂 The Umbrella Project', 'font-size: 20px; font-weight: bold; color: #0866FF;');
console.log('%cEnvironmental Intelligence Platform', 'font-size: 14px; color: #65676B;');
console.log('%cWebsite built with ❤️ for sustainable technology', 'font-size: 12px; color: #8A8D91;');

// Cookie Consent Handling
document.addEventListener('DOMContentLoaded', () => {
    const cookieConsent = document.getElementById('cookie-consent');
    const acceptButton = document.getElementById('accept-cookies');
    const declineButton = document.getElementById('decline-cookies');

    // Check if user has already made a choice
    const cookieChoice = localStorage.getItem('cookieConsent');
    
    if (!cookieChoice) {
        // Show cookie banner after a short delay
        setTimeout(() => {
            cookieConsent.classList.add('show');
        }, 1000);
    }

    // Accept cookies
    acceptButton.addEventListener('click', () => {
        localStorage.setItem('cookieConsent', 'accepted');
        cookieConsent.classList.remove('show');
        trackEvent('Cookie', 'Accept', 'User accepted cookies');
    });

    // Decline cookies
    declineButton.addEventListener('click', () => {
        localStorage.setItem('cookieConsent', 'declined');
        cookieConsent.classList.remove('show');
        trackEvent('Cookie', 'Decline', 'User declined cookies');
    });
});
