
const username = 'afonsorodrigues';

window.addEventListener('DOMContentLoaded', () => {
    fetch(`http://192.168.155.20:3000/sessions/${username}`)

        .then(res => res.json())
        .then(sessions => {
            const labels = sessions.map((s, i) => `Sessão ${i + 1}`);
            const data = sessions.map(s => s.volume);

            const ctx = document.getElementById('userChart').getContext('2d');
            new Chart(ctx, {
                type: 'line',
                data: {
                    labels: labels,
                    datasets: [{
                        label: 'Consumo de Água (L)',
                        data: data,
                        borderColor: 'rgba(0, 123, 255, 1)',
                        backgroundColor: 'rgba(0, 123, 255, 0.2)',
                        borderWidth: 2,
                        tension: 0.4
                    }]
                },
                options: {
                    responsive: true,
                    maintainAspectRatio: false,
                    scales: {
                        y: {
                            beginAtZero: true
                        }
                    }
                }
            });
        })
        .catch(err => console.error('Erro ao carregar dados do gráfico:', err));
});
