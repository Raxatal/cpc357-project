const firebaseBaseURL =
  "https://project-iot-98b30-default-rtdb.firebaseio.com";

function fetchData() {
  // Fetch distance
  fetch(firebaseBaseURL + "/ultrasonic/distance.json")
    .then(res => res.json())
    .then(distance => {
      document.getElementById("distance").innerText =
        distance.toFixed(2) + " cm";

      evaluateStatus(distance);
    });

  // Fetch flow
  fetch(firebaseBaseURL + "/flow/rate.json")
    .then(res => res.json())
    .then(flow => {
      document.getElementById("flow").innerText =
        flow.toFixed(3) + " L/min";
    });
}

function evaluateStatus(distance) {
  const card = document.getElementById("statusCard");
  const text = document.getElementById("statusText");

  // Reset card classes
  card.className = "card status";

  if (distance > 6.0) {
    text.innerText = "Normal Flow";
    card.classList.add("green");
  } else if (distance > 4.0) {
    text.innerText = "Partial Blockage Detected";
    card.classList.add("yellow");
  } else {
    text.innerText = "CRITICAL BLOCKAGE!";
    card.classList.add("red");
  }
}

// Refresh every 2 seconds
setInterval(fetchData, 2000);
fetchData();
