document.querySelector('#blinkForm').addEventListener('submit', (e) => {
  e.preventDefault();

  const postData = {
    blink: document.querySelector('#shouldBlink').checked,
    blinkRate: document.querySelector('#blinkRate').value,
  };
  const headers = {
    method: 'PATCH',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(postData),
  };

  fetch('/blink', headers).then(res => console.log(res));
});
