var dropdownHandler = document.querySelector('.dropdown-handler')
var dropdownParams = document.querySelector('.dropdown-params')
function toggleClass(element) {
  element.classList.toggle('active')
}

dropdownHandler.addEventListener('click', function() {
  toggleClass(dropdownHandler)
  toggleClass(dropdownParams)
})