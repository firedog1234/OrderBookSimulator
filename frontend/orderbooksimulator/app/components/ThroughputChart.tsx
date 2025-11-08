"use client";

import React from "react";
import {
  Card,
  CardContent,
  Typography,
  Box,
} from "@mui/material";
import {
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
import { formatThroughput, formatNumber } from "@/app/utils/formatters";

interface ThroughputChartProps {
  rbTree?: Metrics;
  skipList?: Metrics;
}

export default function ThroughputChart({
  rbTree,
  skipList,
}: ThroughputChartProps) {
  const data = [
    {
      name: "Red-Black Tree",
      throughput: rbTree?.ordersPerSecond || 0,
    },
    {
      name: "Skip List",
      throughput: skipList?.ordersPerSecond || 0,
    },
  ];

  return (
    <Box>
      <Typography variant="h6" component="h3" gutterBottom sx={{ mb: 2 }}>
        Throughput Comparison
      </Typography>
      <Card>
        <CardContent>
          <ResponsiveContainer width="100%" height={300}>
            <BarChart data={data}>
              <CartesianGrid strokeDasharray="3 3" />
              <XAxis dataKey="name" />
              <YAxis
                label={{ value: "Throughput (orders/sec)", angle: -90, position: "left", offset: 10 }}
                tickFormatter={(value: number) => formatNumber(Math.round(value))}
                width={80}
              />
              <Tooltip
                formatter={(value: number) => formatThroughput(value)}
              />
              <Legend />
              <Bar dataKey="throughput" fill="#1976d2" />
            </BarChart>
          </ResponsiveContainer>
        </CardContent>
      </Card>
    </Box>
  );
}

