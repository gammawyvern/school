import * as THREE from 'three';
const scene = new THREE.Scene();

let boxes = [];

const fov = 75;
const near = 0.1;
const far = 1000;
const camera = new THREE.PerspectiveCamera(fov, window.innerWidth / window.innerHeight, near, far);

const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);
renderer.setClearColor(0x000000);

// Load textures
let floor_width = 20;
let floor_height = 40;
let box_width = 0.9;
let box_height = 2;
let box_depth = 0.2;
let texture_loader = new THREE.TextureLoader(); 

// Load in textures
let portal = texture_loader.load("./assets/portal_block.png");
let grass = texture_loader.load("./assets/grass_block.png");

grass.wrapS = THREE.RepeatWrapping;
grass.wrapT = THREE.RepeatWrapping;
let floor_ratio = floor_width / floor_height; 
grass.repeat.set(40*floor_ratio, 40);

portal.wrapS = THREE.RepeatWrapping;
portal.wrapT = THREE.RepeatWrapping;
let box_ratio = box_width / box_height;
portal.repeat.set(6*box_ratio, 6);

// Add floor mesh
let floor_geometry = new THREE.PlaneGeometry(floor_width, floor_height);
let floor_material = new THREE.MeshStandardMaterial({
  map: grass,
  metalness: 0,
  roughness: 0.5
});
let floor_mesh = new THREE.Mesh(floor_geometry, floor_material);
floor_mesh.position.set(0, -1, -3);
floor_mesh.rotation.set(-Math.PI/2, 0, 0);
scene.add(floor_mesh);

// Add torus mesh 
let torus_geometry = new THREE.TorusGeometry(1, 0.5, 12, 48);
let torus_material = new THREE.MeshStandardMaterial({
  map: portal
});
let torus_mesh = new THREE.Mesh(torus_geometry, torus_material);
torus_mesh.receiveShadow = true;
torus_mesh.castShadow = true;
torus_mesh.position.set(0, 1, -5);
scene.add(torus_mesh);

// Add boxe meshes to scene
let box_geometry = new THREE.BoxGeometry(box_width, box_height, box_depth);
let box_material = new THREE.MeshStandardMaterial({
  map: portal,
  metalness: 0,
  roughness: 0
});
for (let i = 0; i < 14; i++) {
  let box_mesh = new THREE.Mesh(box_geometry, box_material);
  box_mesh.position.set(i - 7, 1, -10);
  box_mesh.receiveShadow = true;
  box_mesh.castShadow = true;
  scene.add(box_mesh);
  boxes.push(box_mesh);
}

// Camera / Lighting
let ambient = new THREE.AmbientLight(0x111111);
scene.add(ambient);
let light_intensity = 25;
let point_light = new THREE.PointLight(0xffffff, light_intensity);
point_light.position.set(0, 5, -5);
scene.add(point_light);
renderer.shadowMap.enabled = true;
floor_mesh.receiveShadow = true;
point_light.castShadow = true;

// Run animation loop
function animate() {
  for(let i = 0; i < boxes.length; i++) {
    boxes[i].rotateX(Math.PI/(100+i));
  }

  torus_mesh.rotateX(Math.PI/(200));
  torus_mesh.rotateY(Math.PI/(300));

  requestAnimationFrame(animate);
  renderer.render(scene, camera);
}

animate();

