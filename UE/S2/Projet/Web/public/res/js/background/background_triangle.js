class Background {

	/* initialise openGL, appelé dans le constructeur */
	initGL() {
		/* le fragment shader */
		var FRAGMENT_SHADER_SRC = `
			#ifdef GL_ES
				precision highp float;
			#endif

			uniform float cursorX;
			uniform float cursorY;
			uniform float time;

			varying float pass_color;
			varying float pass_x;
			varying float pass_y;

			void main() {
				float dx = cursorX - pass_x;
				float dy = (cursorY - pass_y) / 1.6;
				float d = sqrt(dx * dx + dy * dy);
				gl_FragColor = vec4(1.0, 1.0, 1.0, pass_color);
			}
		`;

		/* le vertex shader */
		var VERTEX_SHADER_SRC =`
			attribute vec2 position;
			attribute float color;

			varying float pass_color;
			varying float pass_x;
			varying float pass_y;

			void main(void) {
				gl_Position = vec4(position.x, position.y, 0.0, 1.0);
				pass_color = color;
				pass_x = position.x;
				pass_y = position.y;
			}
		`;

		gl.viewport(0, 0, canvas.width, canvas.height);
		gl.clearColor(0.2, 0.21, 0.28, 1.0);
		gl.clear(gl.COLOR_BUFFER_BIT);

		gl.enable(gl.DEPTH_TEST)

		gl.enable(gl.BLEND);
		gl.blendFunc(gl.SRC_ALPHA, gl.ONE);

		/* create shaders */
		this.vs = gl.createShader(gl.VERTEX_SHADER);
		this.fs = gl.createShader(gl.FRAGMENT_SHADER);
		gl.shaderSource(this.vs, VERTEX_SHADER_SRC);
		gl.shaderSource(this.fs, FRAGMENT_SHADER_SRC);
		gl.compileShader(this.vs);
		gl.compileShader(this.fs);
	    if (!gl.getShaderParameter(this.vs, gl.COMPILE_STATUS)) {
	        alert("vertex shader error : " + gl.getShaderInfoLog(this.vs));
	    }
	    if (!gl.getShaderParameter(this.fs, gl.COMPILE_STATUS)) {
	        alert("fragment shader error : " + gl.getShaderInfoLog(this.fs));
	    }


		/* create rendering program */
		this.program = gl.createProgram();
		gl.attachShader(this.program, this.vs);
		gl.attachShader(this.program, this.fs);
		gl.linkProgram(this.program);
		/* use the program */
		gl.useProgram(this.program);


	    /* create vbo */
	    this.vbo = gl.createBuffer();
	    gl.bindBuffer(gl.ARRAY_BUFFER, this.vbo);
	    var attrPosition	= gl.getAttribLocation(this.program, "position");
	    var attrColor		= gl.getAttribLocation(this.program, "color");
	    var step	= 3 * Float32Array.BYTES_PER_ELEMENT;
	    var offset	= 0;

	    gl.vertexAttribPointer(attrPosition, 	2, gl.FLOAT, false,	step, offset);
	    gl.enableVertexAttribArray(attrPosition);
	    offset += 2 * Float32Array.BYTES_PER_ELEMENT;
	    
	    gl.vertexAttribPointer(attrColor,		1, gl.FLOAT, false,	step, offset);
	    gl.enableVertexAttribArray(attrColor);


	    /* uniforms */
	   	this.u_cursorX = gl.getUniformLocation(this.program, "cursorX");
	   	this.u_cursorY = gl.getUniformLocation(this.program, "cursorY");
	   	this.u_time    = gl.getUniformLocation(this.program, "time");

	   	/* line width */
	   	gl.lineWidth(2.0);
	}


	// TODO : when to call it?
	deinitGL() {
		gl.deleteShader(this.fs);
		gl.deleteShader(this.vs);
		gl.deleteProgram(this.program);
		gl.deleteBuffer(this.vbo);
	}

	// TODO : optimize this with a single buffer and draw indices
	/* intialises les triangles */
	initTriangles() {
		/* initialisation de la grille */
		var positions = [];
		var nx = 16 * 1;
		var ny = 9 * 1;
		var w = 2.0 / nx;
		var h = 2.0 / ny;
		var vx = w * 0.25;
		var vy = h * 0.25;
		 for (var i = 0; i <= nx; i++) {
	    	for (var j = 0; j <= ny; j++) {
	    		var x = i * w + (2.0 * Math.random() * vx - vx) - 1.0;
	    		var y = j * h + (2.0 * Math.random() * vy - vy) - 1.0;

	    		positions.push([x, y]);
	    	}
		}
		var triangles = Delaunator.from(positions).triangles;

		/* generate vbo data */
	    var vertices = [];
		for (var i = 0 ; i < triangles.length; i += 3) {
			var x1 = positions[triangles[i + 0]][0];
			var y1 = positions[triangles[i + 0]][1];

			var x2 = positions[triangles[i + 1]][0];
			var y2 = positions[triangles[i + 1]][1];

			var x3 = positions[triangles[i + 2]][0];
			var y3 = positions[triangles[i + 2]][1];

			var c = 0.6 + Math.random() * 0.1;

			var v1 = [x1, y1, c];
			var v2 = [x2, y2, c];
			var v3 = [x3, y3, c];

			vertices.push.apply(vertices, v1);
			vertices.push.apply(vertices, v2);
			vertices.push.apply(vertices, v3);
	
	/*
	 * vertices.push.apply(vertices, v1); vertices.push.apply(vertices, v2);
	 * 
	 * vertices.push.apply(vertices, v2); vertices.push.apply(vertices, v3);
	 * 
	 * vertices.push.apply(vertices, v3); vertices.push.apply(vertices, v1);
	 */
		}

	    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
		this.vertexCount = vertices.length / 3;

	}

	/* constructeur par défaut */
	constructor() {
		/* context du cavans sur lequel dessiné */
		this.ctx = canvas.getContext("2d");

		/* initialisation OpenGL */
		this.initGL();

		/* generation des points / triangles */
		this.initTriangles();
	}

	/**
	 * Met à jour le fond d'écran
	 * 
	 * @param dt :
	 *            temps entre le dernier appel de cette fonction et maintenant
	 */
	 update(dt) {
	 	/*
		 * for (var i = 0 ; i < this.origins.length ; i++) { var x =
		 * this.origins[i][0]; var y = this.origins[i][1]; var dx = cursorX - x;
		 * var dy = cursorY - y; var d = dx * dx + dy * dy; if (d < 200) { d =
		 * 200; } var offx = -2000 * dx / d; var offy = -2000 * dy / d;
		 * this.positions[i][0] = x + offx; this.positions[i][1] = y + offy; }
		 */
	}

	/**
	 * Met à jour le canvas sur lequel le fond est dessiné
	 */
	 draw() {
		/* clear screen */
		gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

		/* draw */
		gl.uniform1f(this.u_cursorX, 2.0 * cursorX / canvas.width - 1.0);
		gl.uniform1f(this.u_cursorY, 2.0 * (1.0 - cursorY / canvas.height) - 1.0);
		var ms = new Date().getMilliseconds();
		if (ms > 500) {
			ms = 1000 - ms;
		}
		var t = ms / 500.0 * 3.1418 * 2.0;
		gl.uniform1f(this.u_time, t);

	// gl.drawArrays(gl.LINES, 0, this.vertexCount);
		gl.drawArrays(gl.TRIANGLES, 0, this.vertexCount);

	    var err = gl.getError();
	    if (err != gl.NO_ERROR) {
	        console.log("GL error occured: " + prefix + " : " + err);
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
	onResize();
	gl = canvas.getContext("experimental-webgl");

	/* canvas.style.webkitFilter = "blur(3px)"; */
}

function onResize() {
	canvas.width = window.innerWidth;
	canvas.height = window.innerHeight;
}

function initMouse() {
	onCursorMove(canvas.width / 2, canvas.height / 2);
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