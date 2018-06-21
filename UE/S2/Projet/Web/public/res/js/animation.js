/** une bibliothèque de fonctions pratiques pour rendre les pages plus dynamiques */

function emptyFunction() {}

/**
 * crée une animation sur un élément
 * 
 * @param element :
 *            l'element html
 * @param fps :
 *            le nombre de fps pour l'animation
 * @param duration :
 *            durée de l'animation
 * @param onUpdate :
 *            une fonction appelé à chaque frame de l'animation
 * @param onEnded :
 *            une fonction appelé à la fin de l'animation
 */
function animate(element, fps, duration, onUpdate, onEnded=emptyFunction) {
	var f  = 0.0;
	var df = 1.0 / (fps * duration);
	var timer = setInterval(function() {
		if (f >= 1.0) {
			f = 1.0;
			clearInterval(timer);
			onEnded();
		}
		onUpdate(f);
		f += df;
	}, 1000.0 / fps);
}

/**
 * rends un élément invisible progressivement - element : l'élément - fps : le
 * nombre de fps de l'animation - duration : la durée de la transition
 * 
 * @param element :
 *            l'element html
 * @param fps :
 *            le nombre de fps pour l'animation
 * @param la
 *            durée de l'animation
 */
function fadeOut(element, fps, duration, onUpdate=emptyFunction, onEnded=emptyFunction) {
	animate(element, fps, duration,
			function(f) {
				element.style.opacity = 1.0 - f;
				element.style.filter = "alpha(opacity=" + ((1.0 - f) * 100.0) + ")";
				onUpdate(f);
			},

			function() {
				element.style.display = "none";
				onEnded();
			}
	);
}

/**
 * rends un élément visible progressivement - element : l'élément - fps : le
 * nombre de fps de l'animation - duration : la durée de la transition
 * 
 * @param element :
 *            l'element html
 * @param fps :
 *            le nombre de fps pour l'animation
 * @param duration :
 *            durée de l'animation
 */
function fadeIn(element, fps, duration, onUpdate=emptyFunction, onEnded=emptyFunction) {
	element.style.display = "";
	element.style.opacity = 0.0;
	element.style.filter = "alpha(opacity=0)";

	animate(element, fps, duration,
			function(f) {
				element.style.opacity = f;
				element.style.filter = "alpha(opacity=" + (f * 100.0) + ")";
				onUpdate(f);
			},

			function() {
				element.style.display = "";
				onEnded();
			}
	);
}

/**
 * animation de translation d'un element
 * 
 * @param element :
 *            l'element html
 * @param fps :
 *            le nombre de fps pour l'animation
 * @param duration :
 *            durée de l'animation
 * @param offx :
 *            translation x final
 * @param offy :
 *            translation y final
 */
function translate(element, fps, duration=0.5, offx=0.0, offy=2.0, onUpdate=emptyFunction, onEnded=emptyFunction) {

	animate(element, fps, duration,
			function(f) {
				var tx = "translateX(" + ((offx > 0) ? (1.0 - f) * offx : -f * offx) + "rem)";
				var ty = "translateY(" + ((offy > 0) ? (1.0 - f) * offy : -f * offy) + "rem)";
				element.style.transform = tx + " " + ty;
				onUpdate(f);
			},

			function() {
				element.style.transform = "";
				onEnded();
			}
	);
}