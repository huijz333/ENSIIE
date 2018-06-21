/**
 * rediriges vers une autre page du site
 */
function redirectToPage(page) {
    window.location.replace("?page=" + page);
}

/**
 * rediriges vers une autre page externe au site
 */
function redirect(domain) {
    window.location.replace(domain);
}
