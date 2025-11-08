# Order Book Simulator Dashboard

A Next.js TypeScript dashboard for visualizing performance metrics from the Order Book Simulator C++ backend.

## Features

- Side-by-side comparison of Red-Black Tree vs Skip List performance metrics
- Real-time latency visualization with percentile charts
- Throughput and memory usage comparisons
- Operation breakdown (Add/Cancel/Modify) with pie and bar charts
- Comprehensive performance comparison table
- Responsive Material UI design

## Tech Stack

- **Next.js 14+** (App Router)
- **TypeScript**
- **Material UI (MUI)** v5+
- **Recharts** for data visualization

## Setup

1. Install dependencies:
```bash
npm install
# or
yarn install
```

2. Make sure the C++ backend is running on `http://localhost:8080`

3. Run the development server:
```bash
npm run dev
# or
yarn dev
```

4. Open [http://localhost:3000](http://localhost:3000) in your browser

## API Endpoint

The dashboard expects the backend API to return metrics in one of these formats:

- Array format: `[{...rbTree metrics...}, {...skipList metrics...}]`
- Object format: `{rbTree: {...}, skipList: {...}}`

The API endpoint should be available at `http://localhost:8080/`

## Project Structure

```
frontend/orderbooksimulator/
├── app/
│   ├── components/          # React components
│   │   ├── Dashboard.tsx
│   │   ├── MetricsOverview.tsx
│   │   ├── LatencyChart.tsx
│   │   ├── ThroughputChart.tsx
│   │   ├── MemoryUsageChart.tsx
│   │   ├── OperationBreakdown.tsx
│   │   └── PerformanceComparison.tsx
│   ├── services/            # API service layer
│   │   └── api.ts
│   ├── types/               # TypeScript type definitions
│   │   └── metrics.ts
│   ├── utils/               # Utility functions
│   │   └── formatters.ts
│   ├── layout.tsx           # Root layout with MUI theme
│   ├── page.tsx             # Main dashboard page
│   └── theme.ts             # Material UI theme configuration
├── package.json
├── tsconfig.json
└── next.config.js
```

## Notes

- The backend API must enable CORS if running on a different port
- The dashboard handles loading and error states gracefully
- All metrics are formatted appropriately (latency in µs, memory in KB/MB/GB, etc.)

