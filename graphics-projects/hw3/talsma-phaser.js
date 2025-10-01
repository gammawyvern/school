let player;
let cursors;

const config = {
    type: Phaser.AUTO,
    width: 800,
    height: 600,
    scene: {
        preload: preload,
        create: create,
        update: update
    }
};

const game = new Phaser.Game(config);

function preload() {
  this.load.image("player", "./assets/player_sprite.png");
}

function create() {
  player = this.add.sprite(400, 300, "player");
  player.texture.setFilter(Phaser.Textures.FilterMode.NEAREST); 
  player.setScale(4);
  // player.setCollideWorldBounds(true);

  cursors = this.input.keyboard.createCursorKeys();
}

function move() {
  player.x -= cursors.left.isDown ? 4 : 0;
  player.x += cursors.right.isDown ? 4 : 0;

  player.y -= cursors.up.isDown ? 4 : 0;
  player.y += cursors.down.isDown ? 4 : 0;

  if (player.x < player.width) {
    player.x = player.width;
  } else if (player.x > (config.width - player.width)) {
    player.x = config.width - player.width;
  }

  if (player.y < player.height) {
    player.y = player.height;
  } else if (player.y > (config.height - player.height)) {
    player.y = config.height - player.height;
  }
}

function update() {
  move();
}

