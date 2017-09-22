
function glhCreateShader(gl, id) {
	var shaderScript = document.getElementById(id);
    if (!shaderScript) {
        return (null);
    }

    var src = shaderScript.firstChild.nodeValue;
    var shader;
    if (shaderScript.type == "fs") {
        shader = gl.createShader(gl.FRAGMENT_SHADER);
    } else if (shaderScript.type == "vs") {
        shader = gl.createShader(gl.VERTEX_SHADER);
    } else if (shaderScript.type == "gs") {
        shader = gl.createShader(gl.GEOMETRY_SHADER);
    } else {
    	alert("not a valid shader type");
    }

    gl.shaderSource(shader, src);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        alert(gl.getShaderInfoLog(shader));
        return (null);
    }

    return (shader);
}

function glhCheckError(gl, prefix) {
    var err = gl.getError();
    if (err != gl.NO_ERROR) {
        alert("GL error occured: " + prefix + " : " + err);
    }
}
