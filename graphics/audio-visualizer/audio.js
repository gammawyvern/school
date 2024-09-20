function setupAudioVisualizer() {
  const audioContext = new (window.AudioContext || window.webkitAudioContext)();
  const analyser = audioContext.createAnalyser();
  analyser.fftSize = 256;

  const audioPlayer = document.getElementById('audio-controller');
  const fileInput = document.getElementById('audio-file');
  const source = audioContext.createMediaElementSource(audioPlayer);
  source.connect(analyser);
  analyser.connect(audioContext.destination);

  fileInput.addEventListener('change', function(event) {
    const file = event.target.files[0];

    if (file) {
      audioPlayer.src = URL.createObjectURL(file);

      audioPlayer.play().then(() => {
        console.log('Audio playback started');
        visualizeFrequencyData(analyser);
      }).catch(err => {
        console.error('Error starting audio playback:', err);
      });
    }
  });
}

function visualizeFrequencyData(analyser) {
  const canvas = document.getElementById('visualizer');
  const ctx = canvas.getContext('2d');

  const bufferLength = analyser.frequencyBinCount;
  const dataArray = new Uint8Array(bufferLength);

  function draw() {
    analyser.getByteFrequencyData(dataArray);

    ctx.clearRect(0, 0, canvas.width, canvas.height);

    const barWidth = (canvas.width / bufferLength) * 2.5;
    let barHeight;
    let x = 0;

    for (let i = 0; i < bufferLength; i++) {
      barHeight = dataArray[i];

      ctx.fillStyle = 'rgb(0, 153, 242)';
      ctx.fillRect(x, canvas.height - barHeight / 2, barWidth, barHeight / 2);

      x += barWidth + 1;
    }

    requestAnimationFrame(draw);
  }

  draw();
}

window.onload = function() {
  setupAudioVisualizer();
};

