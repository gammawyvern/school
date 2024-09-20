var gl;
var points;

let anim_speed = 0.02;
let triangle_scale = 0.25;

let translation_loc;
let translation = vec2(0.0, 0.0);
let velocity = vec2(0.8, 0.6);

let rotation_loc;
let rotation = 0.0;
let rotation_velocity = Math.PI;

function getRandomArbitrary(min, max) {
  return Math.random() * (max - min) + min;
}

window.onload = function init() {
  var canvas = document.getElementById('gl-canvas');
  gl = WebGLUtils.setupWebGL(canvas);

  if (!gl) { alert('WebGL unavailable'); }

  var vertices = [
    scale(triangle_scale, vec2( 0,  1)),
    scale(triangle_scale, vec2( 1, -1)),
    scale(triangle_scale, vec2(-1, -1))
  ];

  gl.viewport(0, 0, canvas.width, canvas.height);

  var program = initShaders(gl, 'vertex-shader', 'fragment-shader');
  gl.useProgram(program);

  var bufferID = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, bufferID);
  gl.bufferData(gl.ARRAY_BUFFER, flatten(vertices), gl.STATIC_DRAW);

  translation_loc = gl.getUniformLocation(program, 'translation');
  rotation_loc = gl.getUniformLocation(program, 'rotation');

  var vPosition = gl.getAttribLocation(program, 'vPosition');
  gl.vertexAttribPointer(vPosition, 2, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(vPosition);
  render();
};

function logic() {
  translation = add(translation, scale(anim_speed, velocity));
  rotation += anim_speed * rotation_velocity;

  // Using a variable velocity, such as delta time, would break this
  if (Math.abs(translation[0]) > 0.9) {
    velocity[0] = -velocity[0];
    rotation_velocity = -rotation_velocity;
  }

  // Using a variable velocity, such as delta time, would break this
  if (Math.abs(translation[1]) > 0.9) {
    velocity[1] = -velocity[1];
    rotation_velocity = -rotation_velocity;
  }
}

function render() {
  logic();

  gl.uniform2fv(translation_loc, translation);
  gl.uniform1f(rotation_loc, rotation);

  gl.clear(gl.COLOR_BUFFER_BIT);
  gl.drawArrays(gl.TRIANGLES, 0, 3);

  // Cap run-rate to 50 fps
  setTimeout(function() {
    window.requestAnimationFrame(render);
  }, 20);
}

