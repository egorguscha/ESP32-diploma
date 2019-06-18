var ws = new WebSocket('ws://192.168.4.1')
var button = document.querySelector('.params-list')
var conenctStatus = document.getElementById('connect-status')
var formButton = document.getElementById('button-send')
var input = document.getElementById('input-field')
var on = document.querySelector('on')
var off = document.querySelector('off')
var paramsList = document.querySelector('.params-list')
var handlerDd = document.querySelectorAll('.dropdown-handler')
var freq = document.getElementById('range-freq')

function toggleClass(element) {
  element.classList.toggle('active')
}

for (var i = 0; i < handlerDd.length; i++) {
  handlerDd[i].addEventListener('click', function(event) {
    var cur = event.target
    var parent = cur.closest('li')
    var dd = parent.lastElementChild

    toggleClass(dd)
    toggleClass(parent)
  })
}

///
function handleControl(event) {
  event.preventDefault()
  var element = event.target
  var dataCommand = element.getAttribute('data-command')
  if (dataCommand) {
    ws.send(dataCommand)
    console.log(dataCommand)
  }
}

function handleRange(event) {
  var value = event.target.value

  ws.send(value)
}

ws.onopen = function() {
  ws.send('Test message to ESP')
  button.addEventListener('click', handleControl)
  freq.addEventListener('change', handleRange)
}

ws.onclose = function() {
  conenctStatus.innerHTML = 'disconnected!'
}

ws.onmessage = function(msg) {
  console.log(msg)
}
