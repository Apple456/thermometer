#!/usr/bin/env bash
set -euox pipefail

APP_NAME="$(buildkite-agent meta-data get app-name)"
echo "************* APPNAME $APP_NAME"
ENV=$1
echo "************* ENV $ENV"

echo "
  - label: Sync new relic alerts for $APP_NAME for $ENV environments
    command:
      - echo "./gradlew $APP_NAME:build $APP_NAME:syncNewRelicAlerts -Penv=$ENV -i"
    retry:
      manual:
        permit_on_passed: true"
