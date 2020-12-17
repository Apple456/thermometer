#!/usr/bin/env bash
set -euox pipefail

APP_NAME="$(buildkite-agent meta-data get app-name)"
if [ "$PRODENVIRONMENT" = true ]; then syncEnv="Prod"; else syncEnv="nonProd"; fi


echo "
  - label: Sync new relic alerts for $APP_NAME for $syncEnv environments
    command:
      - .buildkite/docker/run-java-build.sh ./gradlew $APP_NAME:build $APP_NAME:syncNewRelicAlerts -Penv=$ENV -i
    agents:
      queue: \"payments-build\"
    retry:
      manual:
        permit_on_passed: true"
