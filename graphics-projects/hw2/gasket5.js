"use strict";

var canvas;
var gl;

var points = [];
let p1 = vec2( 0,  1);
let p2 = vec2( 1, -1);
let p3 = vec2(-1, -1);

var numTimesToSubdivide = 0;

function init() {
  canvas = document.getElementById( "gl-canvas" );

  gl = WebGLUtils.setupWebGL( canvas );
  if ( !gl ) { alert( "WebGL isn't available" ); }

  canvas.addEventListener("mouseup", (event) => {
    let rect = gl.canvas.getBoundingClientRect();
    let new_x = (event.clientX - rect.left) / canvas.width * 2 - 1;
    let new_y = (event.clientY - rect.top) / canvas.height * -2 + 1;

    let vertex_id = document.querySelector('input[name="vertex"]:checked').value;
    if (vertex_id == 0) {
      p1 = vec2(new_x, new_y);
    } else if (vertex_id == 1) {
      p2 = vec2(new_x, new_y);
    } else {
      p3 = vec2(new_x, new_y);
    }

    console.log(new_x, new_y);
  });

  var vertices = [
    p1, p2, p3
  ];

  divideTriangle(
    vertices[0],
    vertices[1],
    vertices[2],
    numTimesToSubdivide
  );

  gl.viewport( 0, 0, canvas.width, canvas.height );

  var program = initShaders( gl, "vertex-shader", "fragment-shader" );
  gl.useProgram( program );

  var bufferId = gl.createBuffer();
  gl.bindBuffer( gl.ARRAY_BUFFER, bufferId );
  gl.bufferData( gl.ARRAY_BUFFER, 50000, gl.STATIC_DRAW );
  gl.bufferSubData(gl.ARRAY_BUFFER, 0, flatten(points));

  var vPosition = gl.getAttribLocation( program, "vPosition" );
  gl.vertexAttribPointer( vPosition, 2, gl.FLOAT, false, 0, 0 );
  gl.enableVertexAttribArray( vPosition );

  document.getElementById("slider").onchange = (event) => {
    numTimesToSubdivide = parseInt(event.target.value);
  };

  render();
};

function triangle(a, b, c) {
  points.push(a, b, c);
}

function divideTriangle( a, b, c, count ) {
  if ( count === 0 ) {
    triangle( a, b, c );
  } else {
    var ab = mix( a, b, 0.5 );
    var ac = mix( a, c, 0.5 );
    var bc = mix( b, c, 0.5 );

    --count;

    divideTriangle( a, ab, ac, count );
    divideTriangle( c, ac, bc, count );
    divideTriangle( b, bc, ab, count );
  }
}

window.onload = init;

function render() {
  gl.clear( gl.COLOR_BUFFER_BIT );
  gl.drawArrays( gl.TRIANGLES, 0, points.length );
  points = [];
  requestAnimFrame(init);
}
