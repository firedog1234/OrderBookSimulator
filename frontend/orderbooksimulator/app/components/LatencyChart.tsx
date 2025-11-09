"use client";

import React from "react";
import { Card, CardContent, Typography, Box, Grid } from "@mui/material";
import {
  LineChart,
  Line,
  BarChart,
  Bar,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  Legend,
  ResponsiveContainer,
} from "recharts";
import { Metrics } from "@/app/types/metrics";
import { formatLatency } from "@/app/utils/formatters";

interface LatencyChartProps {
  metrics: Metrics;
  color: string;
}

export default function LatencyChart({ metrics, color }: LatencyChartProps) {
  const percentileData = [
    {
      name: "Min",
      value: metrics.minLatency,
    },
    {
      name: "Median",
      value: metrics.medianLatency,
    },
    {
      name: "P95",
      value: metrics.percentile95,
    },
    {
      name: "P99",
      value: metrics.percentile99,
    },
    {
      name: "Max",
      value: metrics.maxLatency,
    },
  ];

  // Prepare latency distribution data
  const latencyDistribution = metrics.latencies
    .map((latency, index) => ({
      index,
      latency,
    }))
    .slice(0, 100); // Limit to first 100 for readability

  return (
    <Box>
      <Typography
        variant="h6"
        component="h3"
        gutterBottom
        sx={{ color, fontWeight: 600, mb: 2 }}
      >
        Latency Metrics
      </Typography>

      <Grid container spacing={2}>
        <Grid item xs={12} md={6}>
          <Card>
            <CardContent>
              <Typography variant="subtitle1" gutterBottom>
                Latency Percentiles
              </Typography>
              <ResponsiveContainer width="100%" height={300}>
                <BarChart data={percentileData}>
                  <CartesianGrid strokeDasharray="3 3" />
                  <XAxis dataKey="name" />
                  <YAxis
                    label={{
                      value: "Latency (µs)",
                      angle: -90,
                      position: "left",
                      offset: 0,
                    }}
                    tickFormatter={(value: number) => value.toFixed(1)}
                    width={90}
                  />
                  <Tooltip
                    formatter={(value: number) => formatLatency(value)}
                  />
                  <Bar dataKey="value" fill={color} />
                </BarChart>
              </ResponsiveContainer>
            </CardContent>
          </Card>
        </Grid>
        {latencyDistribution.length > 0 && (
          <Grid item xs={12} md={6}>
            <Card>
              <CardContent>
                <Typography variant="subtitle1" gutterBottom>
                  Latency Distribution (First 100)
                </Typography>
                <ResponsiveContainer width="100%" height={300}>
                  <LineChart data={latencyDistribution}>
                    <CartesianGrid strokeDasharray="3 3" />
                    <XAxis dataKey="index" />
                    <YAxis
                      label={{
                        value: "Latency (µs)",
                        angle: -90,
                        position: "left",
                        offset: 0,
                      }}
                      tickFormatter={(value: number) => value.toFixed(1)}
                      width={90}
                    />
                    <Tooltip
                      formatter={(value: number) => formatLatency(value)}
                    />
                    <Line
                      type="monotone"
                      dataKey="latency"
                      stroke={color}
                      dot={false}
                    />
                  </LineChart>
                </ResponsiveContainer>
              </CardContent>
            </Card>
          </Grid>
        )}
      </Grid>
    </Box>
  );
}
