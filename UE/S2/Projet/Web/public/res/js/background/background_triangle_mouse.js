/** Le fond */
var Background = function() {

	/** context dans lequel dessiné */
	this.ctx = canvas.getContext("2d");
	this.origins = [];
	this.positions = [];
	this.radius = [];

	var nx = 18;
	var ny = 12;
	var w = canvas.width / nx;
	var h = canvas.height / ny;
	var vx = w * 0.25;
	var vy = h * 0.25;
	 for (var i = 0; i <= nx; i++) {
    	for (var j = 0; j <= ny; j++) {
    		var x = i * w + 2.0 * Math.random() * vx - vx;
    		var y = j * h + 2.0 * Math.random() * vy - vy;
    		this.origins.push([Math.floor(x), Math.floor(y)]);
    		this.positions.push([0, 0]);
    		this.radius.push(1.0 + Math.random() * 2.0);
    	}
	}
	this.triangles = Delaunator.from(this.origins).triangles;

	this.update = function(dt) {
		for (var i = 0 ; i < this.origins.length ; i++) {
			var x = this.origins[i][0];
			var y = this.origins[i][1];
			var dx = cursorX - x;
			var dy = cursorY - y;
			var d = dx * dx + dy * dy;
			if (d < 200) {
				d = 200;
			}
			var offx = -1000 * dx / d;
			var offy = -1000 * dy / d;
			this.positions[i][0] = x + offx;
			this.positions[i][1] = y + offy;
		}
	}

	/** dessines une ligne */
	this.drawLine = function(x1, y1, x2, y2, lineWidth) {
		this.ctx.lineWidth = lineWidth;
		var a = 1.0 - ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) / 30000;
		if (a < 0) {
			a = 0;
		} else if (a > 0.2) {
			a = 0.2;
		}
		this.ctx.strokeStyle = "rgba(255, 255, 255, " + a + ")";
		this.ctx.beginPath();
		this.ctx.moveTo(x1, y1);
		this.ctx.lineTo(x2, y2);
		this.ctx.stroke();
	}

	/** dessines un cercle */
	this.drawCircle = function(x0, y0, r) {
		this.ctx.fillStyle = "rgba(255, 255, 255, 0.5)";
		this.ctx.beginPath();
		this.ctx.arc(x0, y0, r, 0, 2 * Math.PI, false);
		this.ctx.fill();
	}

	/** dessines le fond d'écran animé */
	this.draw = function() {
		this.ctx.fillStyle = "rgb(182, 185, 202)"
		this.ctx.fillRect(0, 0, canvas.width, canvas.height);

		/** dessines les arrêtes */
		for (var i = 0 ; i < this.triangles.length; i += 3) {
			var x1 = this.positions[this.triangles[i + 0]][0];
			var y1 = this.positions[this.triangles[i + 0]][1];

			var x2 = this.positions[this.triangles[i + 1]][0];
			var y2 = this.positions[this.triangles[i + 1]][1];

			var x3 = this.positions[this.triangles[i + 2]][0];
			var y3 = this.positions[this.triangles[i + 2]][1];

			this.drawLine(x1, y1, x2, y2, 2);
			this.drawLine(x2, y2, x3, y3, 2);
			this.drawLine(x3, y3, x1, y1, 2);
		}

		/** dessine les points */
		for (var i = 0 ; i < this.positions.length ; i++) {
			this.drawCircle(this.positions[i][0], this.positions[i][1], this.radius[i]);
		}
	}
}

/** appelé au chargement de la page */
function onPageLoaded() {
	initCanvas();
	initMouse();
	initLoop();
	loop();
}

/** initialisations */

function initCanvas() {
	canvas = document.getElementById("bgCanvasID");
	/* canvas.style.webkitFilter = "blur(3px)"; */
	onResize();
}

function onResize() {
	canvas.width = window.innerWidth;
	canvas.height = window.innerHeight;
}

function initMouse() {
	onCursorMove(-1000, -1000);
	document.onmousemove = function(e) {
		onCursorMove(e.pageX, e.pageY);
	}
}

function onCursorMove(x, y) {
	cursorX = x;
	cursorY = y;
}

function initLoop() {
	now = Date.now();
	then = Date.now();
	fps = 60.0;
	interval = 1.0 / fps;
	timeElapsed = 0;
	background = new Background();
	window.onresize = onResize;
}

/** boucle de dessin */
async function loop() {
	requestAnimationFrame(loop);

	/* l'heure actuelle */
	var now = Date.now();
	/* l'heure du dernier dessin */
	var dt = (now - then) / 1000.0;
	/* on incremente le temps total */
	timeElapsed += dt;
	/* on met à jour, on dessine */
	background.update(dt);
	background.draw();
	then = now;
	/** on attends le temps du prochain dessin */
	await new Promise(resolve => setTimeout(resolve, interval));

}