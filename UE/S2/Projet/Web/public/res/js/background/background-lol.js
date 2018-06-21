class Background {

	/* initialise openGL, appelé dans le constructeur */
	initGL() {
		/* le fragment shader */
		var FRAGMENT_SHADER_SRC = `
			#ifdef GL_ES
				precision highp float;
			#endif


			//
			// Description : Array and textureless GLSL 2D simplex noise function.
			//      Author : Ian McEwan, Ashima Arts.
			//  Maintainer : stegu
			//     Lastmod : 20110822 (ijm)
			//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
			//               Distributed under the MIT License. See LICENSE file.
			//               https://github.com/ashima/webgl-noise
			//               https://github.com/stegu/webgl-noise
			// 

			vec3 mod289(vec3 x) {
			  return x - floor(x * (1.0 / 289.0)) * 289.0;
			}

			vec2 mod289(vec2 x) {
			  return x - floor(x * (1.0 / 289.0)) * 289.0;
			}

			vec3 permute(vec3 x) {
			  return mod289(((x*34.0)+1.0)*x);
			}

			float snoise(vec2 v)
			  {
			  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
			                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
			                     -0.577350269189626,  // -1.0 + 2.0 * C.x
			                      0.024390243902439); // 1.0 / 41.0
			// First corner
			  vec2 i  = floor(v + dot(v, C.yy) );
			  vec2 x0 = v -   i + dot(i, C.xx);

			// Other corners
			  vec2 i1;
			  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
			  //i1.y = 1.0 - i1.x;
			  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
			  // x0 = x0 - 0.0 + 0.0 * C.xx ;
			  // x1 = x0 - i1 + 1.0 * C.xx ;
			  // x2 = x0 - 1.0 + 2.0 * C.xx ;
			  vec4 x12 = x0.xyxy + C.xxzz;
			  x12.xy -= i1;

			// Permutations
			  i = mod289(i); // Avoid truncation effects in permutation
			  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
					+ i.x + vec3(0.0, i1.x, 1.0 ));

			  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
			  m = m*m ;
			  m = m*m ;

			// Gradients: 41 points uniformly over a line, mapped onto a diamond.
			// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

			  vec3 x = 2.0 * fract(p * C.www) - 1.0;
			  vec3 h = abs(x) - 0.5;
			  vec3 ox = floor(x + 0.5);
			  vec3 a0 = x - ox;

			// Normalise gradients implicitly by scaling m
			// Approximation of: m *= inversesqrt( a0*a0 + h*h );
			  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

			// Compute final noise value at P
			  vec3 g;
			  g.x  = a0.x  * x0.x  + h.x  * x0.y;
			  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
			  return 130.0 * dot(m, g);
			}




			uniform float time;
			uniform sampler2D background;

			varying float pass_x;
			varying float pass_y;
			varying vec2 pass_uv;

			uniform vec2 off1;
			uniform vec2 off2;
			uniform vec2 off3;
			uniform vec2 off4;

			# define TIMER 4.0
			# define MAX_DISTORTION 0.005
			# define DISTORTION_SPEED 0.1

			void main() {
				vec4 color;
				if (time < TIMER) {
					color = texture2D(background, pass_uv);
				} else {
					float distortion = min((time - TIMER) * 0.005, MAX_DISTORTION);

					float nx1 = snoise(pass_uv + off1 * time * DISTORTION_SPEED) * distortion;
					float ny1 = snoise(pass_uv + off2 * time * DISTORTION_SPEED) * distortion;
					vec2 uv1 = pass_uv + vec2(nx1, ny1);

					float nx2 = snoise(pass_uv + off3 * time * DISTORTION_SPEED) * distortion;
					float ny2 = snoise(pass_uv + off4 * time * DISTORTION_SPEED	) * distortion;
					vec2 uv2 = pass_uv + vec2(nx2, ny2);

					color = mix(texture2D(background, uv1), texture2D(background, uv2), 0.5);
				}

				gl_FragColor = vec4(log(color.rgb * 8.0) * 0.2, 0.6);
			}
		`;

		/* le vertex shader */
		var VERTEX_SHADER_SRC =`
			attribute vec2 position;
			attribute vec2 uv;

			varying float pass_x;
			varying float pass_y;
			varying vec2 pass_uv;

			uniform float cursorX;
			uniform float cursorY;

			void main(void) {

				float x = position.x;
				float y = position.y;
				gl_Position = vec4(x, y, 0.0, 1.0);
				pass_uv = uv;
				pass_x = x;
				pass_y = y;
			}
		`;

	    var err = gl.getError();
	    if (err != gl.NO_ERROR) {
	        console.log("GL error occured: a : " + err);
	    }

		gl.viewport(0, 0, canvas.width, canvas.height);
		gl.clearColor(1.0, 1.0, 1.0, 1.0);
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
	    var attrUv			= gl.getAttribLocation(this.program, "uv");
	    var step	= 4 * Float32Array.BYTES_PER_ELEMENT;
	    var offset	= 0;

	    gl.vertexAttribPointer(attrPosition, 	2, gl.FLOAT, false,	step, offset);
	    gl.enableVertexAttribArray(attrPosition);
	    offset += 2 * Float32Array.BYTES_PER_ELEMENT;

	    gl.vertexAttribPointer(attrUv,			2, gl.FLOAT, false,	step, offset);
	    gl.enableVertexAttribArray(attrUv);
	    offset += 2 * Float32Array.BYTES_PER_ELEMENT;


		/* initialisation de la grille */
		var positions = [];
		var uvs = [];
		var nx = 1;
		var ny = 1;
		var w = 1.0 / nx;
		var h = 1.0 / ny;
		 for (var i = 0; i <= nx; i++) {
	    	for (var j = 0; j <= ny; j++) {
	    		var x = i * w * 2.0 - 1.0;
	    		var y = j * h * 2.0 - 1.0;
	    		var u = i * w;
	    		var v = 1.0 - j * h;
	    		positions.push([x, y]);
	    		uvs.push([u, v]);
	    	}
		}

		var triangles = Delaunator.from(positions).triangles;

		/* generate vbo data */
	    var vertices = [];
		for (var i = 0 ; i < triangles.length; i += 3) {
			var x1 = positions[triangles[i + 0]][0];
			var y1 = positions[triangles[i + 0]][1];
			var u1 = uvs[triangles[i + 0]][0];
			var v1 = uvs[triangles[i + 0]][1];

			var x2 = positions[triangles[i + 1]][0];
			var y2 = positions[triangles[i + 1]][1];
			var u2 = uvs[triangles[i + 1]][0];
			var v2 = uvs[triangles[i + 1]][1];

			var x3 = positions[triangles[i + 2]][0];
			var y3 = positions[triangles[i + 2]][1];
			var u3 = uvs[triangles[i + 2]][0];
			var v3 = uvs[triangles[i + 2]][1];

			var v1 = [x1, y1, u1, v1];
			var v2 = [x2, y2, u2, v2];
			var v3 = [x3, y3, u3, v3];

			vertices.push.apply(vertices, v1);
			vertices.push.apply(vertices, v2);
			vertices.push.apply(vertices, v3);
		}

	  	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
	  	this.vertexCount = vertices.length / 4;
	  	console.log(this.vertexCount);

	    /* uniforms */
	   	this.u_cursorX = gl.getUniformLocation(this.program, "cursorX");
	   	this.u_cursorY = gl.getUniformLocation(this.program, "cursorY");
	   	this.u_time    = gl.getUniformLocation(this.program, "time");
	   	this.u_off1    = gl.getUniformLocation(this.program, "off1");
	   	this.u_off2    = gl.getUniformLocation(this.program, "off2");
	   	this.u_off3    = gl.getUniformLocation(this.program, "off3");
	   	this.u_off4    = gl.getUniformLocation(this.program, "off4");

	   	this.off1 = [2.0 * Math.random() - 1.0, 2.0 * Math.random() - 1.0];
	   	this.off2 = [2.0 * Math.random() - 1.0, 2.0 * Math.random() - 1.0];
	   	this.off3 = [2.0 * Math.random() - 1.0, 2.0 * Math.random() - 1.0];
	   	this.off4 = [2.0 * Math.random() - 1.0, 2.0 * Math.random() - 1.0];


	}

	initTextures() {


	   	/* images */
      	var u_background 	= gl.getUniformLocation(this.program, "background");

		gl.uniform1i(u_background, 	0);
		

		function loadImage(url) {
			var tex = gl.createTexture();
			gl.bindTexture(gl.TEXTURE_2D, tex);

			// let's assume all images are not a power of 2
			gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
			gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
			gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);

			var textureInfo = {
				width: 1,   // we don't know the size until it loads
				height: 1,
				texture: tex,
			};
			var img = new Image();
			img.addEventListener('load', function() {
				textureInfo.width = img.width;
				textureInfo.height = img.height;

				gl.bindTexture(gl.TEXTURE_2D, textureInfo.texture);
				gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, img);
			});
			img.src = url;
			return (textureInfo);
		}
		// this.lolbg = loadImage("./images/lolbg.png");
		this.lolbg = loadImage("./images/lolbg.png");

	   	/* bind texture */
	   	gl.activeTexture(gl.TEXTURE0)
	   	gl.bindTexture(gl.TEXTURE_2D, this.lolbg.texture);
	}


	// TODO : when to call it?
	deinitGL() {
		gl.deleteShader(this.fs);
		gl.deleteShader(this.vs);
		gl.deleteProgram(this.program);
		gl.deleteBuffer(this.vbo);
	}

	/* constructeur par défaut */
	constructor() {
		/* context du cavans sur lequel dessiné */
		this.ctx = canvas.getContext("2d");

		/* initialisation OpenGL */
		this.initGL();

		/* generation des points / triangles */
		this.initTextures();

		/** timer */
		this.timer = 0;
		this.prevCursorX = cursorX;
		this.prevCursorY = cursorY;
	}

	/**
	 * Met à jour le fond d'écran
	 * 
	 * @param dt :
	 *            temps entre le dernier appel de cette fonction et maintenant
	 */
	 update(dt) {
	 	this.timer += dt;
	}

	/**
	 * Met à jour le canvas sur lequel le fond est dessiné
	 */
	 draw() {
		/* clear screen */
		// gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

		/* uniforms */
		gl.uniform1f(this.u_cursorX, 2.0 * cursorX / canvas.width - 1.0);
		gl.uniform1f(this.u_cursorY, 2.0 * (1.0 - cursorY / canvas.height) - 1.0);
		gl.uniform1f(this.u_time, this.timer);
		gl.uniform2f(this.u_off1, this.off1[0], this.off1[1]);
		gl.uniform2f(this.u_off2, this.off2[0], this.off2[1]);
		gl.uniform2f(this.u_off3, this.off3[0], this.off3[1]);
		gl.uniform2f(this.u_off4, this.off4[0], this.off4[1]);

		/* draw */
		gl.drawArrays(gl.TRIANGLES, 0, this.vertexCount);

	    var err = gl.getError();
	    if (err != gl.NO_ERROR) {
	        console.log("GL error occured: " + err);
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
	gl = canvas.getContext("experimental-webgl");
	onResize();

	/* canvas.style.webkitFilter = "blur(3px)"; */
}

function onResize() {
	canvas.width = window.innerWidth;
	canvas.height = window.innerHeight;
	gl.viewport(0, 0, canvas.width, canvas.height);
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