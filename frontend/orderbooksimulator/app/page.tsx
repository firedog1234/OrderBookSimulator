"use client";

import { useEffect, useState } from "react";
import Dashboard from "./components/Dashboard";
import { fetchMetrics } from "./services/api";
import { MetricsResponse } from "./types/metrics";

export default function Home() {
  const [metrics, setMetrics] = useState<MetricsResponse>({});
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | undefined>();

  useEffect(() => {
    const loadMetrics = async () => {
      try {
        setLoading(true);
        setError(undefined);
        const data = await fetchMetrics();
        setMetrics(data);
      } catch (err) {
        setError(
          err instanceof Error
            ? err.message
            : "Failed to fetch metrics from API"
        );
        console.error("Error loading metrics:", err);
      } finally {
        setLoading(false);
      }
    };

    loadMetrics();
  }, []);

  return (
    <Dashboard
      rbTree={metrics.rbTree}
      skipList={metrics.skipList}
      loading={loading}
      error={error}
    />
  );
}

