"use client";

import React from "react";
import {
  Card,
  CardContent,
  Typography,
  Grid,
  Box,
} from "@mui/material";
import { Metrics } from "@/app/types/metrics";
import {
  formatLatency,
  formatMemory,
  formatThroughput,
  formatNumber,
} from "@/app/utils/formatters";

interface MetricsOverviewProps {
  metrics: Metrics;
  color: string;
}

export default function MetricsOverview({
  metrics,
  color,
}: MetricsOverviewProps) {
  return (
    <Box>
      <Typography
        variant="h5"
        component="h2"
        gutterBottom
        sx={{ color, fontWeight: 600, mb: 2 }}
      >
        {metrics.name}
      </Typography>
      <Grid container spacing={2}>
        <Grid item xs={12} sm={6} md={4}>
          <Card>
            <CardContent>
              <Typography color="textSecondary" gutterBottom variant="body2">
                Avg Insert Time
              </Typography>
              <Typography variant="h6" component="div">
                {formatLatency(metrics.avgInsertTime)}
              </Typography>
            </CardContent>
          </Card>
        </Grid>

        <Grid item xs={12} sm={6} md={4}>
          <Card>
            <CardContent>
              <Typography color="textSecondary" gutterBottom variant="body2">
                Avg Delete Time
              </Typography>
              <Typography variant="h6" component="div">
                {formatLatency(metrics.avgDeleteTime)}
              </Typography>
            </CardContent>
          </Card>
        </Grid>

        <Grid item xs={12} sm={6} md={4}>
          <Card>
            <CardContent>
              <Typography color="textSecondary" gutterBottom variant="body2">
                Avg Lookup Time
              </Typography>
              <Typography variant="h6" component="div">
                {formatLatency(metrics.avgLookupTime)}
              </Typography>
            </CardContent>
          </Card>
        </Grid>
        <Grid item xs={12} sm={6} md={4}>
          <Card>
            <CardContent>
              <Typography color="textSecondary" gutterBottom variant="body2">
                Orders Per Second
              </Typography>
              <Typography variant="h6" component="div">
                {formatThroughput(metrics.ordersPerSecond)}
              </Typography>
            </CardContent>
          </Card>
        </Grid>

        <Grid item xs={12} sm={6} md={4}>
          <Card>
            <CardContent>
              <Typography color="textSecondary" gutterBottom variant="body2">
                Total Orders
              </Typography>
              <Typography variant="h6" component="div">
                {formatNumber(metrics.totalOrders)}
              </Typography>
            </CardContent>
          </Card>
        </Grid>
        <Grid item xs={12} sm={6} md={4}>
          <Card>
            <CardContent>
              <Typography color="textSecondary" gutterBottom variant="body2">
                Memory Usage
              </Typography>
              <Typography variant="h6" component="div">
                {formatMemory(metrics.memoryUsageBytes)}
              </Typography>
            </CardContent>
          </Card>
        </Grid>
        <Grid item xs={12} sm={6} md={4}>
          <Card>
            <CardContent>
              <Typography color="textSecondary" gutterBottom variant="body2">
                Add Count
              </Typography>
              <Typography variant="h6" component="div">
                {formatNumber(metrics.addCount)}
              </Typography>
            </CardContent>
          </Card>
        </Grid>

        <Grid item xs={12} sm={6} md={4}>
          <Card>
            <CardContent>
              <Typography color="textSecondary" gutterBottom variant="body2">
                Cancel Count
              </Typography>
              <Typography variant="h6" component="div">
                {formatNumber(metrics.cancelCount)}
              </Typography>
            </CardContent>
          </Card>
        </Grid>

        <Grid item xs={12} sm={6} md={4}>
          <Card>
            <CardContent>
              <Typography color="textSecondary" gutterBottom variant="body2">
                Modify Count
              </Typography>
              <Typography variant="h6" component="div">
                {formatNumber(metrics.modifyCount)}
              </Typography>
            </CardContent>
          </Card>
        </Grid>
      </Grid>
    </Box>
  );
}

