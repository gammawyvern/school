let gl;
let points;
let triangle_size = 0.1;

let translation_location;
let rotation_location;

let last_time = 0.0;
let rotation_speed = 2 * Math.PI;
let move_speed = 2;

const rotation_input = {
  clockwise: false,
  counter_clockwise: false
};

const translation_input = {
  forwards: false,
  backwards: false
}

let translation = vec2(0.0, 0.0);
let translation_direction_offset = Math.PI / 2;
let rotation = 0.0;

let vertices = [
  scale(triangle_size, vec2(0, 1)),
  scale(triangle_size, vec2(Math.sqrt(3)/2, -1/2)),
  scale(triangle_size, vec2(-Math.sqrt(3)/2, -1/2)),
];

let colors = [
  vec3(0.0, 1.0, 0.0),
  vec3(0.0, 0.6, 0.95),
  vec3(0.0, 0.6, 0.95),
];

window.onload = function init() {

  window.addEventListener("keydown", function(e) {
    if (e.code === "ArrowLeft") {
      rotation_input.counter_clockwise = true; 
    } else if (e.code === "ArrowRight") {
      rotation_input.clockwise = true;
    } 

    if (e.code === "ArrowDown") {
      translation_input.backwards = true;
    } else if (e.code === "ArrowUp") {
      translation_input.forwards = true; 
    } 
  }, false);

  window.addEventListener("keyup", function(e) {
    if (e.code === "ArrowLeft") {
      rotation_input.counter_clockwise = false; 
    } else if (e.code === "ArrowRight") {
      rotation_input.clockwise = false; 
    }

    if (e.code === "ArrowDown") {
      translation_input.backwards = false;
    } else if (e.code === "ArrowUp") {
      translation_input.forwards = false;
    }
  }, false);

  var canvas = document.getElementById('gl-canvas');
  gl = WebGLUtils.setupWebGL(canvas);

  if (!gl) { alert('WebGL unavailable'); }

  gl.viewport(0, 0, canvas.width, canvas.height);
  // gl.clearColor(0.5, 0.5, 0.5, 0.0);

  var program = initShaders(gl, 'vertex-shader', 'fragment-shader');
  gl.useProgram(program);

  translation_location = gl.getUniformLocation(program, "translation");
  rotation_location = gl.getUniformLocation(program, "rotation")

  var bufferID = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, bufferID);
  gl.bufferData(gl.ARRAY_BUFFER, flatten(vertices), gl.STATIC_DRAW);

  var vPosition = gl.getAttribLocation(program, 'vPosition');
  gl.vertexAttribPointer(vPosition, 2, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(vPosition);

  var c_buffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, c_buffer);
  gl.bufferData(gl.ARRAY_BUFFER, flatten(colors), gl.STATIC_DRAW);

  var v_color = gl.getAttribLocation(program, "v_color");
  gl.vertexAttribPointer(v_color, 3, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(v_color);

  render();
};

function logic(dt) {
  let frame_rotation = dt * rotation_speed;
  let frame_translation = dt * move_speed;

  // Rotation update
  let rotation_dir = 0;
  rotation_dir += rotation_input.clockwise ? 1 : 0;
  rotation_dir -= rotation_input.counter_clockwise ? 1 : 0;
  rotation += rotation_dir * frame_rotation;

  // Translation update
  let translation_dir = 0.0;
  translation_dir += translation_input.forwards ? 1 : 0;
  translation_dir -= translation_input.backwards ? 1 : 0;

  translation_x = translation_dir * frame_translation * -Math.cos(rotation + translation_direction_offset); 
  translation_y = translation_dir * frame_translation * Math.sin(rotation + translation_direction_offset); 
  translation = add(translation, vec2(translation_x, translation_y))
}

function render(current_time) {
  // Avoid weird problems
  if (isNaN(current_time)) {
    window.requestAnimationFrame(render);
    return;
  }

  let delta_time_ms = (current_time - last_time) / 1000;
  logic(delta_time_ms);

  gl.uniform2fv(translation_location, translation);
  gl.uniform1f(rotation_location, rotation);

  gl.clear(gl.COLOR_BUFFER_BIT);
  gl.drawArrays(gl.TRIANGLES, 0, 3);

  last_time = current_time;
  window.requestAnimationFrame(render);
}

