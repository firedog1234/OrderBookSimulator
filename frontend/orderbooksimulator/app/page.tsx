"use client";

import { useState } from "react";
import Dashboard from "./components/Dashboard";
import HomeScreen from "./components/HomeScreen";
import { submitOrderAmount } from "./services/api";
import { MetricsResponse } from "./types/metrics";

export default function Home() {
  const [metrics, setMetrics] = useState<MetricsResponse>({});
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | undefined>();
  const [showDashboard, setShowDashboard] = useState(false);

  const handleSubmit = async (amount: number) => {
    try {
      setLoading(true);
      setError(undefined);
      const data = await submitOrderAmount(amount);
      setMetrics(data);
      setShowDashboard(true);
    } catch (err) {
      setError(
        err instanceof Error ? err.message : "Failed to process order amount"
      );
      console.error("Error submitting order amount:", err);
    } finally {
      setLoading(false);
    }
  };

  const handleBackToHome = () => {
    setShowDashboard(false);
    setMetrics({});
    setError(undefined);
  };

  if (showDashboard) {
    return (
      <Dashboard
        rbTree={metrics.rbTree}
        skipList={metrics.skipList}
        loading={loading}
        error={error}
        onBackToHome={handleBackToHome}
      />
    );
  }

  return <HomeScreen onSubmit={handleSubmit} loading={loading} error={error} />;
}
