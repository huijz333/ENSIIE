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

			void main() {
				gl_FragColor = vec4(pass_color, pass_color, pass_color, 1.0);
			}
		`;

		/* le vertex shader */
		var VERTEX_SHADER_SRC =`
			attribute vec2 position;
			attribute float color;

			varying float pass_color;

			void main(void) {
				gl_Position = vec4(position.x, position.y, 0.0, 1.0);
				pass_color = color;
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


		// create rendering program
		this.program = gl.createProgram();
		gl.attachShader(this.program, this.vs);
		gl.attachShader(this.program, this.fs);
		gl.linkProgram(this.program);
		/* use the program */
		gl.useProgram(this.program);

		// create vao
		this.vao = gl.createVertexArray();
		gl.bindVertexArray(this.vao);

	    // create vbo

	    // positions
	    this.positionsVbo = gl.createBuffer();
	    gl.bindBuffer(gl.ARRAY_BUFFER, this.positionsVbo);
	    var attrPos = gl.getAttribLocation(this.program, "position");
	    gl.vertexAttribPointer(attrPos, 2, gl.FLOAT, false, 2 * Float32Array.BYTES_PER_ELEMENT, 0);
	    gl.enableVertexAttribArray(attrPos);

	    // colors
	    this.colorsVbo = gl.createBuffer();
	    gl.bindBuffer(gl.ARRAY_BUFFER, this.colorsVbo);
	    var attrColor = gl.getAttribLocation(this.program, "color");
	    gl.vertexAttribPointer(attrColor, 1, gl.FLOAT, false, 1 * Float32Array.BYTES_PER_ELEMENT, 0);
	    gl.enableVertexAttribArray(attrColor);

	    // indices
	    this.indexVbo = gl.createBuffer();

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
		gl.deleteBuffer(this.positionsVbo);
		gl.deleteBuffer(this.colorsVbo);
		gl.deleteBuffer(this.indexVbo);
	}

	// TODO : optimize this with a single buffer and draw indices
	/* intialises les triangles */
	initTriangles() {
		/* initialisation de la grille */
		noise.seed(Math.random());
		this.colors = [];
		this.positions = [];
		this.nx = 16 * 1;
		this.ny = 9 * 1;
		var w = 2.0 / this.nx;
		var h = 2.0 / this.ny;
		var vx = w * 0.25;
		var vy = h * 0.25;
		 for (var i = 0; i <= this.nx; i++) {
	    	for (var j = 0; j <= this.ny; j++) {
				
				this.colors.push(0);

	    		var x = i * w + (2.0 * Math.random() * vx - vx) - 1.0;
	    		var y = j * h + (2.0 * Math.random() * vy - vy) - 1.0;
	    		this.positions.push([x, y]);

	    	}
		}
		var triangles = Delaunator.from(this.positions).triangles;
		this.positions = [].concat.apply([], this.positions);

	    gl.bindBuffer(gl.ARRAY_BUFFER, this.positionsVbo);
	    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(this.positions), gl.STATIC_DRAW);

	    gl.bindBuffer(gl.ARRAY_BUFFER, this.colorsVbo);
	    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(this.colors), gl.STATIC_DRAW);

		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexVbo);
		gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(triangles), gl.STATIC_DRAW);
		this.indexCount = triangles.length;


	   // gl.bindBuffer(gl.ARRAY_BUFFER, this.lineVbo);
	    // gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(lineVertices),
		// gl.STATIC_DRAW);
		// this.linesVertexCount = lineVertices.length / 3;
	}

	/* constructeur par défaut */
	constructor() {

		/* initialisation OpenGL */
		this.initGL();

		/* generation des points / triangles */
		this.initTriangles();

		this.acc = 0;
		this.offx = 0.0;
		this.offy = 0.0;
		this.vx = 0.005 * (2.0 * Math.random() - 1.0);
		this.vy = 0.005 * (2.0 * Math.random() - 1.0);
	}

	/**
	 * Met à jour le fond d'écran
	 * 
	 * @param dt :
	 *            temps entre le dernier appel de cette fonction et maintenant
	 */
	update(dt) {

		this.offx += this.vx * dt;
		this.offy += this.vy * dt;

		this.acc += dt;

		var frequency = 64.0;
		for (var i = 0; i <= this.nx; i++) {
	    	for (var j = 0; j <= this.ny; j++) {
	    		var index = i * this.ny + j;
	    		var x = i / this.nx;
	    		var y = j / this.ny;
	    		var f =  0.5 * (noise.perlin2((x + this.offx) * frequency, (y + this.offy) * frequency) + 1.0);
				this.colors[index] = 0.4 + 0.4 * f;
				if (this.colors[index] < 0.0) {
					console.log(this.colors[index]);
				}
	    	}
		}
	    gl.bindBuffer(gl.ARRAY_BUFFER, this.colorsVbo);
	    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(this.colors), gl.STATIC_DRAW);
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
		gl.uniform1f(this.u_time, new Date().getMilliseconds());

		gl.bindVertexArray(this.vao);
		gl.drawElements(gl.TRIANGLES, this.indexCount, gl.UNSIGNED_SHORT, 0);

		// gl.bindBuffer(gl.ARRAY_BUFFER, this.lineVbo);
		// gl.drawArrays(gl.LINES, 0, this.linesVertexCount);

	    var err = gl.getError();
	    if (err != gl.NO_ERROR) {
	    	var errors = {};
	    	errors[gl.INVALID_ENUM] 					= "gl.INVALID_ENUM";
	    	errors[gl.INVALID_VALUE] 					= "gl.INVALID_VALUE";
	    	errors[gl.INVALID_OPERATION] 				= "gl.INVALID_OPERATION";
	    	errors[gl.INVALID_FRAMEBUFFER_OPERATION] 	= "gl.INVALID_FRAMEBUFFER_OPERATION";
	    	errors[gl.OUT_OF_MEMORY] 					= "gl.OUT_OF_MEMORY";
	    	errors[gl.CONTEXT_LOST_WEBGL] 				= "gl.CONTEXT_LOST_WEBGL";
	        // console.log("GL error occured : " + errors[err]);
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
	gl = canvas.getContext("webgl2");

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