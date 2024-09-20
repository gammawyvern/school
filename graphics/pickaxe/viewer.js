// Initialize Three.js scene, camera, and renderer
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
renderer.setClearColor(0x6e5342);
document.body.appendChild(renderer.domElement);

// Allow switching between models
let sword_model;
let pickaxe_model;
let on_pickaxe = true;

const loader = new THREE.GLTFLoader();
loader.load("./models/pickaxe.glb", (gltf) => {
  gltf.scene.position.y -= 0.5;
  pickaxe_model = gltf.scene;
  scene.add(pickaxe_model);
});

loader.load("./models/sword.glb", (gltf) => {
  sword_model = gltf.scene;
});


// Add quick swapping
function toggle_model() {
  if (on_pickaxe) {
    scene.remove(pickaxe_model)
    scene.add(sword_model)
  } else {
    scene.remove(sword_model)
    scene.add(pickaxe_model)
  }

  on_pickaxe = !on_pickaxe;
}

document.addEventListener('keydown', (event) => {
    if (event.code === 'Space') {
        toggle_model();
    }
});

// Setup to be able to view models
camera.position.z = 3;
let ambient = new THREE.AmbientLight(0xffffff);
scene.add(ambient);
let light_intensity = 2;
let point_light = new THREE.PointLight(0xffffff, light_intensity);
point_light.position.set(0, 5, 1);
scene.add(point_light);
renderer.shadowMap.enabled = true;
point_light.castShadow = true;

const controls = new THREE.OrbitControls(camera, renderer.domElement);
controls.enableDamping = true; 
controls.dampingFactor = 0.1; 

function animate() {
  controls.update();
  renderer.render(scene, camera);
  requestAnimationFrame(animate);
}

animate();
