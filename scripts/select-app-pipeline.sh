#!/usr/bin/env bash
set -eox pipefail

APP_NAME="$(buildkite-agent meta-data get app-name)"
echo "************* APPNAME $APP_NAME"
ENV="$($1)"
echo "************* ENV $ENV"

  - label: Sync new relic alerts for $APP_NAME for $ENV environments
    command:
      - ./gradlew $APP_NAME:build $APP_NAME:syncNewRelicAlerts -Penv=$ENV -i
    retry:
      manual:
        permit_on_passed: true
