const firebaseBaseURL =
  "https://project-iot-98b30-default-rtdb.firebaseio.com";

let distanceChart, flowChart;

const distanceCtx = document.getElementById('distanceChart').getContext('2d');
const flowCtx = document.getElementById('flowChart').getContext('2d');

// Initialise empty charts
distanceChart = new Chart(distanceCtx, {
    type: 'line',
    data: { labels: [], datasets: [{ label: 'Distance (cm)', data: [], borderColor: 'blue', fill: false }] },
    options: {
        responsive: true,
        scales: {
            x: { title: { display: true, text: 'Time (ms)' } },
            y: { title: { display: true, text: 'Distance (cm)' } }
        }
    }
});

flowChart = new Chart(flowCtx, {
    type: 'line',
    data: { labels: [], datasets: [{ label: 'Flow (L/min)', data: [], borderColor: 'green', fill: false }] },
    options: {
        responsive: true,
        scales: {
            x: { title: { display: true, text: 'Time (ms)' } },
            y: { title: { display: true, text: 'Flow (L/min)' } }
        }
    }
});

function fetchHistory() {
    fetch(firebaseBaseURL + "/history.json")
        .then(res => res.json())
        .then(data => {
            if (!data) return;

            const timestamps = Object.keys(data).sort((a,b)=>a-b);
            const distanceData = [];
            const flowData = [];

            const tbody = document.getElementById('historyTable').getElementsByTagName('tbody')[0];
            tbody.innerHTML = ''; // clear previous table

            timestamps.forEach(ts => {
                const entry = data[ts];
                distanceData.push(entry.distance);
                flowData.push(entry.flow);

                // Table row
                const row = tbody.insertRow();
                row.insertCell(0).innerText = ts;
                row.insertCell(1).innerText = entry.distance.toFixed(2);
                row.insertCell(2).innerText = entry.flow.toFixed(3);
                row.insertCell(3).innerText = entry.status;

                // Color code table row based on status
                if (entry.status.includes("Normal")) row.style.backgroundColor = "#d4edda";      // green
                else if (entry.status.includes("Partial")) row.style.backgroundColor = "#fff3cd"; // yellow
                else row.style.backgroundColor = "#f8d7da";                                      // red
            });

            // Update charts
            distanceChart.data.labels = timestamps;
            distanceChart.data.datasets[0].data = distanceData;
            distanceChart.update();

            flowChart.data.labels = timestamps;
            flowChart.data.datasets[0].data = flowData;
            flowChart.update();
        })
        .catch(err => console.error(err));
}

// Refresh every 2 seconds
setInterval(fetchHistory, 2000);
fetchHistory();
